#ifndef SIMIT_STORAGE_H
#define SIMIT_STORAGE_H

#include <map>
#include <memory>

#include "intrusive_ptr.h"

namespace simit {
namespace ir {

class Func;
class Var;
class Expr;
class Stmt;

/// The storage arrangement of a tensor (e.g. dense or stored on a set).
class TensorStorage {
public:
  enum Kind {
    Undefined,

    /// The tensor is stored in dense row major order.
    DenseRowMajor,

    /// The tensor is stored in dense z (morton) order.
    // DenseZOrder

    /// The matrix is stored sparsely using the compressed sparse row layout.
    // SparseMatrixCSR,

    /// A system tensor that is split in one dimension with one slice stored on
    /// each element of the set of that dimension.
    /// For now we will assume it was split along the first dimension.
    SystemReduced,

    /// A system tensor whose contributions are stored on the target set that it
    /// was assembled from. That is, the tensor is stored prior to the map
    /// reduction, and any expression that uses the tensor must reduce it.
    SystemUnreduced,

    /// A system tensor that is never stored. Any index expressions that use
    /// this tensor must be fused with the tensor assembly.
    SystemNone
  };

  TensorStorage();
  TensorStorage(Kind kind);

  /// Retrieve the tensor storage kind.
  Kind getKind() const;

  /// True if the tensor is stored on a system, false otherwise.
  bool isSystem() const;

  void setSystemStorageSet(const Expr &systemStorageSet);

  const Expr &getSystemTargetSet() const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};
std::ostream &operator<<(std::ostream&, const TensorStorage&);

/// The storage of a set of tensors.
class Storage {
public:
  Storage();

  /// Add storage for a tensor variable.
  void add(const Var &tensor, TensorStorage tensorStorage);

  /// True if the tensor has a storage descriptor, false otherwise.
  bool hasStorage(const Var &tensor) const;

  /// Retrieve the storage of a tensor variable to modify it.
  TensorStorage &get(const Var &tensor);

  /// Retrieve the storage of a tensor variable to inspect it.
  const TensorStorage &get(const Var &tensor) const;

  /// Iterator over storage Vars in this Storage descriptor.
  class Iterator {
  public:
    struct Content;
    Iterator(Content *content);
    ~Iterator();
    const Var &operator*();
    const Var *operator->();
    Iterator& operator++();
    friend bool operator!=(const Iterator&, const Iterator&);
  private:
    Content *content;
  };

  /// Get an iterator pointing to the first Var in this Storage.
  Iterator begin() const;

  /// Get an iterator pointing to the last Var in this Storage.
  Iterator end() const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};
std::ostream &operator<<(std::ostream&, const Storage&);

/// Retrieve a storage descriptor for each tensor used in 'func'.
Storage getStorage(const Func &func);

}}

#endif
