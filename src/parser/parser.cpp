// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.



# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.h"

// User implementation prologue.


// Unqualified %code blocks.


  #include <stdlib.h>
  #include <cassert>
  #include <iostream>
  #include <map>
  #include <set>
  #include <algorithm>

  #include "program_context.h"
  #include "scanner.h"
  #include "ir_codegen.h"
  using namespace std;
  using namespace simit::internal;
  using namespace simit::ir;

  std::string typeString(const Type &type) {
    std::stringstream ss;
    ss << type;
    std::string str = ss.str();
    if (type.isTensor() && tensorTypePtr(&type)->isColumnVector()) {
      str += "'";
    }
    return str;
  }

  #define REPORT_ERROR(msg, loc)  \
    do {                          \
      error((loc), (msg));        \
      YYERROR;                    \
    } while (0)

  #define REPORT_TYPE_MISSMATCH(t1, t2, loc)                          \
    do {                                                              \
      std::stringstream errorStr;                                     \
      errorStr << "type missmatch (" <<                               \
                  typeString(t1) << " and " << typeString(t2) << ")"; \
      REPORT_ERROR(errorStr.str(), loc);                              \
    } while (0)

  #define REPORT_INDEX_VARIABLE_MISSMATCH(numIndexVars, order, loc) \
    do {                                                            \
      REPORT_ERROR("wrong number of index variables (" +            \
                    to_string(numIndexVars) +                       \
                    " index variables, but tensor order is " +      \
                    to_string(order), loc);                         \
      } while (0)

  void Parser::error(const Parser::location_type &loc, const std::string &msg) {
    errors->push_back(Error(loc.begin.line, loc.begin.column,
                            loc.end.line, loc.end.column, msg));
  }

  #undef yylex
  #define yylex scanner->lex

  inline std::string convertAndFree(const char *str) {
    std::string result = std::string(str);
    free((void*)str);
    return result;
  }

  template <typename T>
  inline T convertAndDelete(T *obj) {
    auto result = T(*obj);
    delete obj;
    return result;
  }

  void transposeVector(const std::shared_ptr<Expression> &vec) {
    assert(vec->getType()->isTensor());
    TensorType *ttype = tensorTypePtr(vec->getType());
    assert(ttype->getOrder() == 1);

    auto transposedVector = new TensorType(ttype->getComponentType(),
                                           ttype->getDimensions(),
                                           !ttype->isColumnVector());
    vec->setType(std::shared_ptr<TensorType>(transposedVector));
  }

  bool compare(const Type &l, const Type &r, ProgramContext *ctx) {
    if (l.getKind() != r.getKind()) {
      return false;
    }

    if (l.isTensor()) {
      if (tensorTypePtr(&l)->isColumnVector() !=
          tensorTypePtr(&r)->isColumnVector()) {
        return false;
      }
    }

    if (l != r) {
      return false;
    }
    return true;
  }

  #define CHECK_IS_TENSOR(expr, loc)                    \
    do {                                                \
      assert(expr->getType());                          \
      if (!expr->getType()->isTensor()) {               \
        std::stringstream errorStr;                     \
        errorStr << "expected tensor";                  \
        REPORT_ERROR(errorStr.str(), loc);              \
      }                                                 \
    } while (0)

  #define CHECK_TYPE_EQUALITY(t1, t2, loc)              \
    do {                                                \
      if (!compare(t1, t2, ctx)) {                      \
        REPORT_TYPE_MISSMATCH(t1, t2, loc);             \
      }                                                 \
    } while (0)


  #define BINARY_ELWISE_TYPE_CHECK(lt, rt, loc)         \
    do {                                                \
      const TensorType *ltt = tensorTypePtr(lt);        \
      const TensorType *rtt = tensorTypePtr(rt);        \
      if (ltt->getOrder() > 0 && rtt->getOrder() > 0) { \
        CHECK_TYPE_EQUALITY(*ltt, *rtt, loc);           \
      }                                                 \
    }                                                   \
    while (0)




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace  simit { namespace internal  {


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
   Parser ::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
   Parser :: Parser  (Scanner *scanner_yyarg, ProgramContext *ctx_yyarg, std::vector<Error> *errors_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      ctx (ctx_yyarg),
      errors (errors_yyarg)
  {}

   Parser ::~ Parser  ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
   Parser ::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
   Parser ::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
   Parser ::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
   Parser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
   Parser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
   Parser ::basic_symbol<Base>::~basic_symbol ()
  {
  }

  template <typename Base>
  inline
  void
   Parser ::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
   Parser ::by_type::by_type ()
     : type (empty)
  {}

  inline
   Parser ::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
   Parser ::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
   Parser ::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
   Parser ::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
   Parser ::by_state::by_state ()
    : state (empty)
  {}

  inline
   Parser ::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
   Parser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
   Parser ::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
   Parser ::symbol_number_type
   Parser ::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
   Parser ::stack_symbol_type::stack_symbol_type ()
  {}


  inline
   Parser ::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty;
  }

  inline
   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
   Parser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yysym.type_get ())
    {
            case 5: // "int literal"


        {}

        break;

      case 6: // "float literal"


        {}

        break;

      case 7: // "string literal"


        { free((void*)((yysym.value.string))); }

        break;

      case 8: // "identifier"


        { free((void*)((yysym.value.string))); }

        break;

      case 58: // program_element


        { delete (yysym.value.IRNode); }

        break;

      case 59: // extern


        { delete (yysym.value.expression); }

        break;

      case 60: // element_type_decl


        { delete (yysym.value.elementType); }

        break;

      case 61: // field_decl_list


        { delete (yysym.value.fields); }

        break;

      case 62: // field_decl


        { delete (yysym.value.field); }

        break;

      case 63: // procedure


        { delete (yysym.value.function); }

        break;

      case 66: // procedure_header


        { delete (yysym.value.function); }

        break;

      case 67: // function


        { delete (yysym.value.function); }

        break;

      case 70: // function_header


        { delete (yysym.value.function); }

        break;

      case 71: // arguments


        { delete (yysym.value.arguments); }

        break;

      case 72: // argument_list


        { delete (yysym.value.arguments); }

        break;

      case 73: // argument_decl


        { delete (yysym.value.argument); }

        break;

      case 74: // results


        { delete (yysym.value.results); }

        break;

      case 75: // result_list


        { delete (yysym.value.results); }

        break;

      case 76: // result_decl


        { delete (yysym.value.result); }

        break;

      case 77: // stmt_block


        { delete (yysym.value.IRNodes); }

        break;

      case 78: // stmt


        { delete (yysym.value.IRNodes); }

        break;

      case 79: // const_stmt


        { delete (yysym.value.IRNodes); }

        break;

      case 80: // return_stmt


        { delete (yysym.value.IRNodes); }

        break;

      case 81: // assign_stmt


        { delete (yysym.value.IRNodes); }

        break;

      case 82: // expr_stmt


        { delete (yysym.value.IRNodes); }

        break;

      case 83: // if_stmt


        { delete (yysym.value.IRNodes); }

        break;

      case 86: // expr


        { delete (yysym.value.expression); }

        break;

      case 87: // ident_expr


        { delete (yysym.value.expression); }

        break;

      case 88: // paren_expr


        { delete (yysym.value.expression); }

        break;

      case 89: // linear_algebra_expr


        { delete (yysym.value.indexExpr); }

        break;

      case 90: // elwise_binary_op


        {}

        break;

      case 91: // boolean_expr


        { delete (yysym.value.expression); }

        break;

      case 92: // field_read_expr


        { delete (yysym.value.fieldRead); }

        break;

      case 93: // set_read_expr


        { delete (yysym.value.expression); }

        break;

      case 94: // call_or_tensor_read_expr


        { delete (yysym.value.expression); }

        break;

      case 95: // call_expr


        { delete (yysym.value.call); }

        break;

      case 96: // expr_list_or_empty


        { delete (yysym.value.expressions); }

        break;

      case 97: // expr_list


        { delete (yysym.value.expressions); }

        break;

      case 98: // range_expr


        { delete (yysym.value.expression); }

        break;

      case 99: // map_expr


        { delete (yysym.value.expression); }

        break;

      case 103: // write_expr_list


        { delete (yysym.value.writeinfos); }

        break;

      case 104: // write_expr


        { delete (yysym.value.writeinfo); }

        break;

      case 105: // field_write_expr


        { delete (yysym.value.fieldWrite); }

        break;

      case 106: // tensor_write_expr


        { delete (yysym.value.tensorWrite); }

        break;

      case 107: // type


        { delete (yysym.value.type); }

        break;

      case 108: // element_type


        { delete (yysym.value.elementType); }

        break;

      case 109: // set_type


        { delete (yysym.value.setType); }

        break;

      case 110: // endpoints


        { delete (yysym.value.expressions); }

        break;

      case 111: // tuple_type


        { delete (yysym.value.tupleType); }

        break;

      case 112: // tuple_element_types


        { delete (yysym.value.elementTypes); }

        break;

      case 113: // tuple_element_type


        { delete (yysym.value.elementTypes); }

        break;

      case 114: // tensor_type


        { delete (yysym.value.tensorType); }

        break;

      case 115: // nested_dimensions


        { delete (yysym.value.IndexDomains); }

        break;

      case 116: // dimensions


        { delete (yysym.value.IndexSets); }

        break;

      case 117: // dimension


        { delete (yysym.value.indexSet); }

        break;

      case 118: // component_type


        {}

        break;

      case 120: // tensor_literal


        { delete (yysym.value.TensorLiteral); }

        break;

      case 121: // dense_tensor_literal


        { delete (yysym.value.TensorLiteral); }

        break;

      case 122: // float_dense_tensor_literal


        { delete (yysym.value.TensorDoubleValues); }

        break;

      case 123: // float_dense_ndtensor_literal


        { delete (yysym.value.TensorDoubleValues); }

        break;

      case 124: // float_dense_matrix_literal


        { delete (yysym.value.TensorDoubleValues); }

        break;

      case 125: // float_dense_vector_literal


        { delete (yysym.value.TensorDoubleValues); }

        break;

      case 126: // int_dense_tensor_literal


        { delete (yysym.value.TensorIntValues); }

        break;

      case 127: // int_dense_ndtensor_literal


        { delete (yysym.value.TensorIntValues); }

        break;

      case 128: // int_dense_matrix_literal


        { delete (yysym.value.TensorIntValues); }

        break;

      case 129: // int_dense_vector_literal


        { delete (yysym.value.TensorIntValues); }

        break;

      case 130: // scalar_literal


        { delete (yysym.value.TensorLiteral); }

        break;

      case 131: // test


        { delete (yysym.value.test); }

        break;


      default:
        break;
    }
  }

#if YYDEBUG
  template <typename Base>
  void
   Parser ::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
   Parser ::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
   Parser ::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
   Parser ::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   Parser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   Parser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   Parser ::debug_level_type
   Parser ::debug_level () const
  {
    return yydebug_;
  }

  void
   Parser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline  Parser ::state_type
   Parser ::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
   Parser ::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
   Parser ::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
   Parser ::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 5:

    {
    std::unique_ptr<Function> function((yystack_[0].value.function));
    std::string name = function->getName();
    if (ctx->containsFunction(name)) {
      REPORT_ERROR("procedure redefinition (" + name + ")", yystack_[0].location);
    }
    ctx->addFunction(function.release());
  }

    break;

  case 6:

    {
    std::unique_ptr<Function> function((yystack_[0].value.function));
    std::string name = function->getName();
    if (ctx->containsFunction(name)) {
      REPORT_ERROR("function redefinition (" + name + ")", yystack_[0].location);
    }
    ctx->addFunction(function.release());
  }

    break;

  case 7:

    {
    (yylhs.value.IRNode) = NULL;
  }

    break;

  case 8:

    {
    (yylhs.value.IRNode) = NULL;
    delete (yystack_[0].value.IRNodes);
  }

    break;

  case 9:

    {
    ctx->addTest((yystack_[0].value.test));
  }

    break;

  case 10:

    {
    auto externArgument = convertAndDelete((yystack_[1].value.argument));
    ctx->addExtern(externArgument);
  }

    break;

  case 11:

    {
    std::string name = convertAndFree((yystack_[2].value.string));
    std::unique_ptr<ir::ElementType::FieldsMapType> fields((yystack_[1].value.fields));

    if (ctx->containsElementType(name)) {
      REPORT_ERROR("struct redefinition (" + name + ")", yylhs.location);
    }

    ElementType *elementType = new ElementType(name, *fields);
    ctx->addElementType(std::shared_ptr<ElementType>(elementType));
  }

    break;

  case 12:

    {
    (yylhs.value.fields) = new ElementType::FieldsMapType();
  }

    break;

  case 13:

    {
    (yylhs.value.fields) = (yystack_[1].value.fields);
    (yylhs.value.fields)->insert(*(yystack_[0].value.field));
    delete (yystack_[0].value.field);
  }

    break;

  case 14:

    {
    std::string name = convertAndFree((yystack_[3].value.string));
    std::shared_ptr<TensorType> tensorType = convertAndDelete((yystack_[1].value.tensorType));
    (yylhs.value.field) = new pair<string,shared_ptr<ir::TensorType>>(name, tensorType);
  }

    break;

  case 15:

    {ctx->scope();}

    break;

  case 16:

    {ctx->unscope();}

    break;

  case 17:

    {
    auto statements = unique_ptr<vector<shared_ptr<IRNode>>>((yystack_[2].value.IRNodes));
    (yylhs.value.function) = (yystack_[3].value.function);
    (yylhs.value.function)->addStatements(*statements);
  }

    break;

  case 18:

    {
    std::string name = convertAndFree((yystack_[0].value.string));
    auto arguments = vector<shared_ptr<Argument>>();
    auto results = vector<shared_ptr<Result>>();

    for (auto &extPair : ctx->getExterns()) {
      auto ext = extPair.second;

      // TODO: Replace extResult with mutable parameters
      auto tmp = new Result(ext->getName(), ext->getType());
      auto extResult = std::shared_ptr<Result>(tmp);
      results.push_back(extResult);

      arguments.push_back(ext);
      ctx->addSymbol(ext->getName(), ext, extResult);
    }

    (yylhs.value.function) = new Function(name, arguments, results);
  }

    break;

  case 19:

    {ctx->scope();}

    break;

  case 20:

    {ctx->unscope();}

    break;

  case 21:

    {
    auto statements = unique_ptr<vector<shared_ptr<IRNode>>>((yystack_[2].value.IRNodes));
    (yylhs.value.function) = (yystack_[3].value.function);
    (yylhs.value.function)->addStatements(*statements);
  }

    break;

  case 22:

    {
    std::string name = convertAndFree((yystack_[4].value.string));
    auto arguments = unique_ptr<vector<shared_ptr<Argument>>>((yystack_[2].value.arguments));
    auto results = unique_ptr<vector<shared_ptr<Result>>>((yystack_[0].value.results));
    (yylhs.value.function) = new Function(name, *arguments, *results);

    std::set<std::string> argumentNames;
    for (auto argument : *arguments) {
      ctx->addSymbol(argument->getName(), argument, NULL);
      argumentNames.insert(argument->getName());
    }

    for (auto result : *results) {
      std::shared_ptr<Expression> readExpr(NULL);
      if (argumentNames.find(result->getName()) != argumentNames.end()) {
        readExpr = ctx->getSymbol(result->getName()).getReadExpr();
      }

      ctx->addSymbol(result->getName(), readExpr, result);
    }
  }

    break;

  case 23:

    {
    (yylhs.value.arguments) = new vector<shared_ptr<Argument>>;
  }

    break;

  case 24:

    {
    (yylhs.value.arguments) = (yystack_[0].value.arguments);
 }

    break;

  case 25:

    {
    auto argument = convertAndDelete((yystack_[0].value.argument));
    (yylhs.value.arguments) = new vector<shared_ptr<Argument>>;
    (yylhs.value.arguments)->push_back(argument);
  }

    break;

  case 26:

    {
    auto argument = convertAndDelete((yystack_[0].value.argument));
    (yylhs.value.arguments) = (yystack_[2].value.arguments);
    (yylhs.value.arguments)->push_back(argument);
  }

    break;

  case 27:

    {
    std::string name = convertAndFree((yystack_[2].value.string));

    auto type = convertAndDelete((yystack_[0].value.type));
    (yylhs.value.argument) = new shared_ptr<Argument>(new Argument(name, type));
  }

    break;

  case 28:

    {
    (yylhs.value.results) = new vector<shared_ptr<Result>>();
  }

    break;

  case 29:

    {
    (yylhs.value.results) = (yystack_[1].value.results);
  }

    break;

  case 30:

    {
    auto result = convertAndDelete((yystack_[0].value.result));
    (yylhs.value.results) = new vector<shared_ptr<Result>>;
    (yylhs.value.results)->push_back(result);
  }

    break;

  case 31:

    {
    auto result = convertAndDelete((yystack_[0].value.result));
    (yylhs.value.results) = (yystack_[2].value.results);
    (yylhs.value.results)->push_back(result);
  }

    break;

  case 32:

    {
    std::string name = convertAndFree((yystack_[2].value.string));
    auto type = convertAndDelete((yystack_[0].value.type));
    (yylhs.value.result) = new shared_ptr<Result>(new Result(name, type));
  }

    break;

  case 33:

    {
    (yylhs.value.IRNodes) = new vector<shared_ptr<IRNode>>();
  }

    break;

  case 34:

    {
    (yylhs.value.IRNodes) = (yystack_[1].value.IRNodes);
    if ((yystack_[0].value.IRNodes) == NULL) break;  // TODO: Remove check
    (yylhs.value.IRNodes)->insert((yylhs.value.IRNodes)->end(), (yystack_[0].value.IRNodes)->begin(), (yystack_[0].value.IRNodes)->end());
    delete (yystack_[0].value.IRNodes);
  }

    break;

  case 40:

    {
    std::string ident = convertAndFree((yystack_[5].value.string));
    std::shared_ptr<TensorType> tensorType = convertAndDelete((yystack_[3].value.tensorType));

    auto literal = shared_ptr<Literal>(*(yystack_[1].value.TensorLiteral));
    delete (yystack_[1].value.TensorLiteral);

    assert(literal->getType()->isTensor() && "Set literals not supported yet");
    auto literalType = literal->getType();

    literal->setName(ident);

    // If tensor_type is a 1xn matrix and $tensor_literal is a vector then we
    // cast $tensor_literal to a 1xn matrix.
    const TensorType *literalTensorType = tensorTypePtr(literalType);
    if (tensorType->getOrder() == 2 && literalTensorType->getOrder() == 1) {
      literal->cast(tensorType);
    }

    // Typecheck: value and literal types must be equivalent.
    CHECK_TYPE_EQUALITY(*tensorType, *literal->getType(), yystack_[5].location);

    ctx->addSymbol(literal->getName(), literal);

    (yylhs.value.IRNodes) = new vector<shared_ptr<IRNode>>();
    (yylhs.value.IRNodes)->push_back(literal);
  }

    break;

  case 41:

    {
    (yylhs.value.IRNodes) = NULL;
  }

    break;

  case 42:

    {
    auto lhsList = unique_ptr<vector<WriteInfo*>>((yystack_[3].value.writeinfos));
    auto rhsList = unique_ptr<vector<shared_ptr<Expression>>>((yystack_[1].value.expressions));

    (yylhs.value.IRNodes) = new vector<shared_ptr<IRNode>>();

    if (lhsList->size() > rhsList->size()) {
      // TODO: Handle maps and then reintroduce this error
      // REPORT_ERROR("too few expressions assigned to too many variables", @2);
      break;
    }
    else if (lhsList->size() < rhsList->size()) {
      REPORT_ERROR("too many expressions assigned to too few variables", yystack_[2].location);
    }

    auto lhsIter = lhsList->begin();
    auto rhsIter = rhsList->begin();
    for (; lhsIter != lhsList->end(); ++lhsIter, ++rhsIter) {
      unique_ptr<WriteInfo> lhs(*lhsIter);
      shared_ptr<Expression> &rhs = *rhsIter;

      if (rhs == NULL) continue;

      switch (lhs->kind) {
        case WriteInfo::Kind::VARIABLE: {
          std::string variableName = *lhs->variableName;

          // TODO: Remove check
          if (!ctx->hasSymbol(variableName)) continue;

          assert(ctx->hasSymbol(variableName));

          const RWExprPair &varExprPair = ctx->getSymbol(variableName);
          assert(varExprPair.isWritable());

          shared_ptr<Expression> lhsTensor = varExprPair.getWriteExpr();
          if (auto result = dynamic_pointer_cast<Result>(lhsTensor)) {
            CHECK_TYPE_EQUALITY(*result->getType(), *rhs->getType(), yystack_[2].location);
            rhs->setName(result->getName());
            result->addValue(rhs);
            (yylhs.value.IRNodes)->push_back(rhs);
          }
          else {
            NOT_SUPPORTED_YET;
          }
          break;
        }
        case WriteInfo::Kind::FIELD: {
          std::shared_ptr<FieldWrite> fieldWrite(*lhs->fieldWrite);
          fieldWrite->setValue(rhs);

          auto result = dynamic_pointer_cast<Result>(fieldWrite->getTarget());
          if (result) {
            result->addValue(fieldWrite);
            (yylhs.value.IRNodes)->push_back(fieldWrite);
          }

          break;
        }
        case WriteInfo::Kind::TENSOR: {
          std::shared_ptr<TensorWrite> tensorWrite(*lhs->tensorWrite);
          tensorWrite->setValue(rhs);

          auto result = dynamic_pointer_cast<Result>(tensorWrite->getTensor());
          if (result){
            result->addValue(tensorWrite);
            (yylhs.value.IRNodes)->push_back(tensorWrite);
          }

          break;
        }
      }
    }
  }

    break;

  case 43:

    {
    (yylhs.value.IRNodes) = NULL;
  }

    break;

  case 44:

    {
    (yylhs.value.IRNodes) = NULL;
  }

    break;

  case 45:

    {
    (yylhs.value.IRNodes) = NULL;
    delete (yystack_[3].value.expression);
    delete (yystack_[2].value.IRNodes);
  }

    break;

  case 47:

    {
    delete (yystack_[0].value.IRNodes);
  }

    break;

  case 49:

    {
    delete (yystack_[1].value.expression);
    delete (yystack_[0].value.IRNodes);
  }

    break;

  case 59:

    {
    string ident = convertAndFree((yystack_[0].value.string));

    if (!ctx->hasSymbol(ident)) { (yylhs.value.expression)=NULL; break; } // TODO: Remove check

    if (!ctx->hasSymbol(ident)) {
      REPORT_ERROR(ident + " is not defined in scope", yystack_[0].location);
    }

    const RWExprPair &rwExprPair = ctx->getSymbol(ident);
    if (!rwExprPair.isReadable()) {
      REPORT_ERROR(ident + " is not readable", yystack_[0].location);
    }

    (yylhs.value.expression) = new shared_ptr<Expression>(rwExprPair.getReadExpr());
  }

    break;

  case 60:

    {
    if ((yystack_[1].value.expression) == NULL) { (yylhs.value.expression) = NULL; break; } // TODO: Remove check
    (yylhs.value.expression) = (yystack_[1].value.expression);
  }

    break;

  case 61:

    {
    if ((yystack_[0].value.expression) == NULL) { (yylhs.value.indexExpr) = NULL; break; } // TODO: Remove check

    std::shared_ptr<Expression> expr = convertAndDelete((yystack_[0].value.expression));
    CHECK_IS_TENSOR(expr, yystack_[0].location);

    (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(unaryElwiseExpr(IndexExpr::NEG, expr));
  }

    break;

  case 62:

    {  // + - .* ./
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.indexExpr) = NULL; break; } // TODO: Remove check

    std::shared_ptr<Expression> l = convertAndDelete((yystack_[2].value.expression));
    std::shared_ptr<Expression> r = convertAndDelete((yystack_[0].value.expression));

    CHECK_IS_TENSOR(l, yystack_[2].location);
    CHECK_IS_TENSOR(r, yystack_[0].location);

    BINARY_ELWISE_TYPE_CHECK(l->getType(), r->getType(), yystack_[1].location);
    (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(binaryElwiseExpr(l, (yystack_[1].value.binop), r));
  }

    break;

  case 63:

    {
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.indexExpr) = NULL; break; } // TODO: Remove check

    std::shared_ptr<Expression> l = convertAndDelete((yystack_[2].value.expression));
    std::shared_ptr<Expression> r = convertAndDelete((yystack_[0].value.expression));

    CHECK_IS_TENSOR(l, yystack_[2].location);
    CHECK_IS_TENSOR(r, yystack_[0].location);

    TensorType *ltype = tensorTypePtr(l->getType());
    TensorType *rtype = tensorTypePtr(r->getType());

    // Scale
    if (ltype->getOrder()==0 || rtype->getOrder()==0) {
      (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(binaryElwiseExpr(l, IndexExpr::MUL, r));
    }
    // Vector-Vector Multiplication (inner and outer product)
    else if (ltype->getOrder() == 1 && rtype->getOrder() == 1) {
      // Inner product
      if (!ltype->isColumnVector()) {
        if (!rtype->isColumnVector()) {
          REPORT_ERROR("cannot multiply two row vectors", yystack_[1].location);
        }
        if (*l->getType() != *r->getType()) {
          REPORT_TYPE_MISSMATCH(*l->getType(), *r->getType(), yystack_[1].location);
        }
        (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(innerProduct(l, r));
      }
      // Outer product (l is a column vector)
      else {
        if (rtype->isColumnVector()) {
          REPORT_ERROR("cannot multiply two column vectors", yystack_[1].location);
        }
        if (*l->getType() != *r->getType()) {
          REPORT_TYPE_MISSMATCH(*l->getType(), *r->getType(), yystack_[1].location);
        }
        (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(outerProduct(l, r));
      }
    }
    // Matrix-Vector
    else if (ltype->getOrder() == 2 && rtype->getOrder() == 1) {
      if (ltype->getDimensions()[1] != rtype->getDimensions()[0]){
        REPORT_TYPE_MISSMATCH(*l->getType(), *rtype, yystack_[1].location);
      }
      (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(gemv(l, r));
    }
    // Vector-Matrix
    else if (ltype->getOrder() == 1 && rtype->getOrder() == 2) {
      if (ltype->getDimensions()[0] != rtype->getDimensions()[0]){
        REPORT_TYPE_MISSMATCH(*ltype, *rtype, yystack_[1].location);
      }
      (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(gevm(l,r));
    }
    // Matrix-Matrix
    else if (ltype->getOrder() == 2 && rtype->getOrder() == 2) {
      if (ltype->getDimensions()[1] != rtype->getDimensions()[0]){
        REPORT_TYPE_MISSMATCH(*ltype, *rtype, yystack_[1].location);
      }
      (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(gemm(l,r));
    }
    else {
      REPORT_ERROR("cannot multiply >2-order tensors using *", yystack_[1].location);
      (yylhs.value.indexExpr) = NULL;
    }
  }

    break;

  case 64:

    {
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.indexExpr) = NULL; break; } // TODO: Remove check
    (yylhs.value.indexExpr) = NULL;
    delete (yystack_[2].value.expression);
    delete (yystack_[0].value.expression);
  }

    break;

  case 65:

    {
    if ((yystack_[1].value.expression) == NULL) { (yylhs.value.indexExpr) = NULL; break; } // TODO: Remove check

    auto expr = shared_ptr<Expression>(*(yystack_[1].value.expression));
    delete (yystack_[1].value.expression);

    CHECK_IS_TENSOR(expr, yystack_[1].location);

    TensorType *type = tensorTypePtr(expr->getType());

    switch (type->getOrder()) {
      case 0:
        // OPT: This might lead to redundant code to be removed in later pass
        (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(unaryElwiseExpr(IndexExpr::NONE, expr));
        break;
      case 1:
        // OPT: This might lead to redundant code to be removed in later pass
        (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(unaryElwiseExpr(IndexExpr::NONE, expr));
        if (!type->isColumnVector()) {
          transposeVector(*(yylhs.value.indexExpr));
        }
        break;
      case 2:
        (yylhs.value.indexExpr) = new shared_ptr<IndexExpr>(transposeMatrix(expr));
        break;
      default:
        REPORT_ERROR("cannot transpose >2-order tensors using '", yystack_[1].location);
        (yylhs.value.indexExpr) = NULL;
    }
  }

    break;

  case 66:

    {
    (yylhs.value.indexExpr) = NULL;
    delete (yystack_[2].value.expression);
    delete (yystack_[0].value.expression);
  }

    break;

  case 67:

    {  // Solve
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.indexExpr) = NULL; break; } // TODO: Remove check
    (yylhs.value.indexExpr) = NULL;
    delete (yystack_[2].value.expression);
    delete (yystack_[0].value.expression);
  }

    break;

  case 68:

    { (yylhs.value.binop) = IndexExpr::ADD; }

    break;

  case 69:

    { (yylhs.value.binop) = IndexExpr::SUB; }

    break;

  case 70:

    { (yylhs.value.binop) = IndexExpr::MUL; }

    break;

  case 71:

    { (yylhs.value.binop) = IndexExpr::DIV; }

    break;

  case 72:

    {
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.expression) = NULL; break; } // TODO: Remove check
    (yylhs.value.expression) = NULL;
    delete (yystack_[2].value.expression);
    delete (yystack_[0].value.expression);
  }

    break;

  case 73:

    {
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.expression) = NULL; break; } // TODO: Remove check
    (yylhs.value.expression) = NULL;
    delete (yystack_[2].value.expression);
    delete (yystack_[0].value.expression);
  }

    break;

  case 74:

    {
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.expression) = NULL; break; } // TODO: Remove check
    (yylhs.value.expression) = NULL;
    delete (yystack_[2].value.expression);
    delete (yystack_[0].value.expression);
  }

    break;

  case 75:

    {
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.expression) = NULL; break; } // TODO: Remove check
    (yylhs.value.expression) = NULL;
    delete (yystack_[2].value.expression);
    delete (yystack_[0].value.expression);
  }

    break;

  case 76:

    {
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.expression) = NULL; break; } // TODO: Remove check
    (yylhs.value.expression) = NULL;
    delete (yystack_[2].value.expression);
    delete (yystack_[0].value.expression);
  }

    break;

  case 77:

    {
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.expression) = NULL; break; } // TODO: Remove check
    (yylhs.value.expression) = NULL;
    delete (yystack_[2].value.expression);
    delete (yystack_[0].value.expression);
  }

    break;

  case 78:

    {
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.string) == NULL) { (yylhs.value.fieldRead) = NULL; break; } // TODO: Remove check
    if ((*(yystack_[2].value.expression))->getType() == NULL) { (yylhs.value.fieldRead) = NULL; break; } // TODO: Remove check

    std::shared_ptr<Expression> elemOrSet = convertAndDelete((yystack_[2].value.expression));
    std::string fieldName = convertAndFree((yystack_[0].value.string));

    auto type = elemOrSet->getType();
    assert(type);
    if (!(type->isElement() || type->isSet())) {
      std::stringstream errorStr;
      errorStr << "only elements and sets have fields";
      REPORT_ERROR(errorStr.str(), yystack_[2].location);
    }

    (yylhs.value.fieldRead) = new shared_ptr<FieldRead>(new FieldRead(elemOrSet, fieldName));
  }

    break;

  case 82:

    {
    std::string name = convertAndFree((yystack_[3].value.string));
    auto indices =
        unique_ptr<vector<shared_ptr<Expression>>>((yystack_[1].value.expressions));

    if (ctx->hasSymbol(name)) {
      const RWExprPair &exprPair = ctx->getSymbol(name);
      if (!exprPair.isReadable()) {
        REPORT_ERROR(name + " is not readable", yystack_[3].location);
      }

      auto tensorExpr = exprPair.getReadExpr();
      (yylhs.value.expression) = new shared_ptr<Expression>(new TensorRead(tensorExpr, *indices));
    }
    else if (ctx->containsFunction(name)) {
      (yylhs.value.expression) = NULL;
    }
    else {
      REPORT_ERROR(name + " is not defined in scope", yystack_[3].location);
    }

  }

    break;

  case 83:

    {
    (yylhs.value.expression) = NULL;
  }

    break;

  case 84:

    {
    std::string name = convertAndFree((yystack_[3].value.string));
    auto actuals =
        unique_ptr<vector<shared_ptr<Expression>>>((yystack_[1].value.expressions));
    auto call = new Call(name, *actuals);
    (yylhs.value.call) = new std::shared_ptr<Call>(call);
  }

    break;

  case 85:

    {
    (yylhs.value.expressions) = new vector<shared_ptr<Expression>>();
  }

    break;

  case 86:

    {
    (yylhs.value.expressions) = (yystack_[0].value.expressions);
  }

    break;

  case 87:

    {
    (yylhs.value.expressions) = new std::vector<std::shared_ptr<Expression>>();
    if ((yystack_[0].value.expression) == NULL) break;  // TODO: Remove check
    (yylhs.value.expressions)->push_back(*(yystack_[0].value.expression));
    delete (yystack_[0].value.expression);
  }

    break;

  case 88:

    {
    (yylhs.value.expressions) = (yystack_[2].value.expressions);
    if ((yystack_[0].value.expression) == NULL) break;  // TODO: Remove check
    (yylhs.value.expressions)->push_back(*(yystack_[0].value.expression));
    delete (yystack_[0].value.expression);
  }

    break;

  case 89:

    {
    if ((yystack_[2].value.expression) == NULL || (yystack_[0].value.expression) == NULL) { (yylhs.value.expression) = NULL; break; } // TODO: Remove check
    (yylhs.value.expression) = NULL;
    delete (yystack_[2].value.expression);
    delete (yystack_[0].value.expression);
  }

    break;

  case 90:

    {
    string function((yystack_[4].value.string));
    string target((yystack_[2].value.string));
    free((void*)(yystack_[4].value.string));
    free((void*)(yystack_[2].value.string));
    (yylhs.value.expression) = NULL;
  }

    break;

  case 92:

    {
    std::string neighbor = convertAndFree((yystack_[0].value.string));
  }

    break;

  case 97:

    {
    (yylhs.value.writeinfos) = new vector<WriteInfo*>();
    if ((yystack_[0].value.writeinfo) == NULL) break;  // TODO: Remove check
    (yylhs.value.writeinfos)->push_back((yystack_[0].value.writeinfo));
  }

    break;

  case 98:

    {
    (yylhs.value.writeinfos) = (yystack_[2].value.writeinfos);
    if ((yystack_[0].value.writeinfo) == NULL) break;  // TODO: Remove check
    (yylhs.value.writeinfos)->push_back((yystack_[0].value.writeinfo));
  }

    break;

  case 99:

    {
    std::string name = convertAndFree((yystack_[0].value.string));
    (yylhs.value.writeinfo) = new WriteInfo(name);
  }

    break;

  case 100:

    {
    (yylhs.value.writeinfo) = new WriteInfo((yystack_[0].value.fieldWrite));
  }

    break;

  case 101:

    {
    (yylhs.value.writeinfo) = new WriteInfo((yystack_[0].value.tensorWrite));
  }

    break;

  case 102:

    {
    string setName = convertAndFree((yystack_[2].value.string));
    string fieldName = convertAndFree((yystack_[0].value.string));

    if(!ctx->hasSymbol(setName)) { (yylhs.value.fieldWrite)=NULL; break; } // TODO: Remove check

    if (!ctx->hasSymbol(setName)) {
      REPORT_ERROR(setName + " is not defined in scope", yystack_[2].location);
    }

    const RWExprPair &setExprPair = ctx->getSymbol(setName);
    if (!setExprPair.isWritable()) {
      REPORT_ERROR(setName + " is not writable", yystack_[2].location);
    }

    auto setExpr = setExprPair.getWriteExpr();
    (yylhs.value.fieldWrite) = new shared_ptr<FieldWrite>(new FieldWrite(setExpr, fieldName));
  }

    break;

  case 103:

    {
    (yylhs.value.fieldWrite) = NULL;
  }

    break;

  case 104:

    {
    std::string tensorName = convertAndFree((yystack_[3].value.string));
    auto indices = unique_ptr<vector<shared_ptr<Expression>>>((yystack_[1].value.expressions));

    if(!ctx->hasSymbol(tensorName)) { (yylhs.value.tensorWrite)=NULL; break; } // TODO: Remove check

    if (!ctx->hasSymbol(tensorName)) {
      REPORT_ERROR(tensorName + " is not defined in scope", yystack_[3].location);
    }

    const RWExprPair &tensorExprPair = ctx->getSymbol(tensorName);
    if (!tensorExprPair.isWritable()) {
      REPORT_ERROR(tensorName + " is not writable", yystack_[3].location);
    }

    auto tensorExpr = tensorExprPair.getWriteExpr();
    (yylhs.value.tensorWrite) = new shared_ptr<TensorWrite>(new TensorWrite(tensorExpr, *indices));
  }

    break;

  case 105:

    {
    // TODO
    (yylhs.value.tensorWrite) = NULL;
  }

    break;

  case 106:

    {
    (yylhs.value.type) = reinterpret_cast<std::shared_ptr<ir::Type>*>((yystack_[0].value.elementType));
  }

    break;

  case 107:

    {
    (yylhs.value.type) = reinterpret_cast<std::shared_ptr<ir::Type>*>((yystack_[0].value.setType));
  }

    break;

  case 108:

    {
    (yylhs.value.type) = reinterpret_cast<std::shared_ptr<ir::Type>*>((yystack_[0].value.tupleType));
  }

    break;

  case 109:

    {
    (yylhs.value.type) = reinterpret_cast<std::shared_ptr<ir::Type>*>((yystack_[0].value.tensorType));
  }

    break;

  case 110:

    {
    std::string name = convertAndFree((yystack_[0].value.string));
    (yylhs.value.elementType) = new std::shared_ptr<ElementType>(ctx->getElementType(name));
  }

    break;

  case 111:

    {
    std::shared_ptr<ElementType> elementType = convertAndDelete((yystack_[1].value.elementType));
    (yylhs.value.setType) = new std::shared_ptr<SetType>(new SetType(elementType));
  }

    break;

  case 112:

    {
    std::shared_ptr<ElementType> elementType = convertAndDelete((yystack_[4].value.elementType));
    auto eps = convertAndDelete((yystack_[1].value.expressions));

    // TODO: Add endpoint information to set type
    (yylhs.value.setType) = new std::shared_ptr<SetType>(new SetType(elementType));
  }

    break;

  case 113:

    {
    (yylhs.value.expressions) = new vector<shared_ptr<Expression>>;
    std::string ident = convertAndFree((yystack_[0].value.string));
  }

    break;

  case 114:

    {
    (yylhs.value.expressions) = (yystack_[2].value.expressions);
    std::string ident = convertAndFree((yystack_[0].value.string));
  }

    break;

  case 115:

    {
    (yylhs.value.tupleType) = new std::shared_ptr<TupleType>(new TupleType(*(yystack_[1].value.elementTypes)));
    delete (yystack_[1].value.elementTypes);
  }

    break;

  case 116:

    {
    (yylhs.value.elementTypes) = (yystack_[0].value.elementTypes);
  }

    break;

  case 117:

    {
    (yylhs.value.elementTypes) = (yystack_[2].value.elementTypes);
    (yylhs.value.elementTypes)->insert((yylhs.value.elementTypes)->begin(), (yystack_[0].value.elementTypes)->begin(), (yystack_[0].value.elementTypes)->end());
    delete (yystack_[0].value.elementTypes);
  }

    break;

  case 118:

    {
    (yylhs.value.elementTypes) = new vector<shared_ptr<ElementType>>;
    (yylhs.value.elementTypes)->push_back(*(yystack_[0].value.elementType));
    delete (yystack_[0].value.elementType);
  }

    break;

  case 119:

    {
    auto elementType = convertAndDelete((yystack_[2].value.elementType));

    if ((yystack_[0].value.num)<1) {
      REPORT_ERROR("Must be 1 or greater", yystack_[0].location);
    }

    (yylhs.value.elementTypes) = new vector<shared_ptr<ElementType>>;
    for (int i=0; i < (yystack_[0].value.num); ++i) {
      (yylhs.value.elementTypes)->push_back(elementType);
    }
    delete (yystack_[2].value.elementType);
  }

    break;

  case 120:

    {
    (yylhs.value.tensorType) = new std::shared_ptr<TensorType>(new TensorType((yystack_[0].value.componentType)));
  }

    break;

  case 121:

    {
    auto nestedDims = unique_ptr<vector<IndexDomain>>((yystack_[3].value.IndexDomains));
    (yylhs.value.tensorType) = new std::shared_ptr<TensorType>(new TensorType((yystack_[1].value.componentType),
                                                        *nestedDims));
  }

    break;

  case 122:

    {
    auto nestedDims = unique_ptr<vector<IndexDomain>>((yystack_[4].value.IndexDomains));
    (yylhs.value.tensorType) = new std::shared_ptr<TensorType>(new TensorType((yystack_[2].value.componentType),
                                                        *nestedDims,
                                                        true));
  }

    break;

  case 123:

    {
    (yylhs.value.IndexDomains) = new std::vector<IndexDomain>();
  }

    break;

  case 124:

    {
    (yylhs.value.IndexDomains) = (yystack_[3].value.IndexDomains);

    auto parentDims = (yylhs.value.IndexDomains);
    auto childDims = unique_ptr<std::vector<IndexSet>>((yystack_[1].value.IndexSets));

    // If there are no previous dimensions then create IndexDomains
    if (parentDims->size() == 0) {
      for (auto &dim : *childDims) {
        UNUSED(dim);
        parentDims->push_back(IndexDomain());
      }
    }

    // Handle case where there are more child than parent dimensions
    if (childDims->size() > parentDims->size()) {
      for (size_t i=0; i < childDims->size() - parentDims->size(); ++i) {
        size_t numNestings = (*parentDims)[0].getFactors().size();
        std::vector<IndexSet> indexSets(numNestings, IndexSet(1));
        parentDims->push_back(IndexDomain(indexSets));
      }
    }

    // Handle case where there are more parent than child dimensions
    if (parentDims->size() > childDims->size()) {
      for (size_t i=0; i < parentDims->size() - childDims->size(); ++i) {
        childDims->push_back(IndexSet(1));
      }
    }

    // Multiply each dimension with the corresponding dimension in the shape.
    assert(childDims->size() == parentDims->size());
    for (size_t i=0; i<(yylhs.value.IndexDomains)->size(); ++i) {
      (*parentDims)[i] = (*parentDims)[i] * IndexDomain((*childDims)[i]);
    }
  }

    break;

  case 125:

    {
    (yylhs.value.IndexSets) = new std::vector<IndexSet>();
    (yylhs.value.IndexSets)->push_back(*(yystack_[0].value.indexSet));
    delete (yystack_[0].value.indexSet);
  }

    break;

  case 126:

    {

    (yylhs.value.IndexSets) = (yystack_[2].value.IndexSets);
    (yylhs.value.IndexSets)->push_back(*(yystack_[0].value.indexSet));
    delete (yystack_[0].value.indexSet);
  }

    break;

  case 127:

    {
    (yylhs.value.indexSet) = new IndexSet((yystack_[0].value.num));
  }

    break;

  case 128:

    {
    string ident = convertAndFree((yystack_[0].value.string));
    (yylhs.value.indexSet) = new IndexSet(ident);
  }

    break;

  case 129:

    {
    (yylhs.value.indexSet) = new IndexSet();
  }

    break;

  case 130:

    {
    (yylhs.value.componentType) = ComponentType::INT;
  }

    break;

  case 131:

    {
    (yylhs.value.componentType) = ComponentType::FLOAT;
  }

    break;

  case 134:

    {
    (yylhs.value.TensorLiteral) = (yystack_[1].value.TensorLiteral);
    transposeVector(*(yylhs.value.TensorLiteral));
  }

    break;

  case 136:

    {
    auto values = unique_ptr<TensorValues<double>>((yystack_[1].value.TensorDoubleValues));
    auto isps = std::vector<IndexDomain>(values->dimSizes.rbegin(),
                                             values->dimSizes.rend());
    auto type = new TensorType(ComponentType::FLOAT, isps);
    auto literal = new Literal(shared_ptr<TensorType>(type), // TODO: <Type>
                               values->values.data());
    (yylhs.value.TensorLiteral) = new shared_ptr<Literal>(literal);
  }

    break;

  case 137:

    {
    auto values = unique_ptr<TensorValues<int>>((yystack_[1].value.TensorIntValues));
    auto isps = std::vector<IndexDomain>(values->dimSizes.rbegin(),
                                             values->dimSizes.rend());
    auto type = new TensorType(ComponentType::INT, isps);
    auto literal = new Literal(shared_ptr<TensorType>(type), // TODO: <Type>
                               values->values.data());
    (yylhs.value.TensorLiteral) = new shared_ptr<Literal>(literal);
  }

    break;

  case 138:

    {
    // If the matrix has only one column then we discard that dimension and
    // treat it as a vector.
    if ((yystack_[0].value.TensorDoubleValues)->dimSizes[(yystack_[0].value.TensorDoubleValues)->dimSizes.size()-1] == 1) {
      (yystack_[0].value.TensorDoubleValues)->dimSizes.pop_back();
    }
  }

    break;

  case 140:

    {
    (yylhs.value.TensorDoubleValues) = (yystack_[1].value.TensorDoubleValues);
    (yylhs.value.TensorDoubleValues)->addDimension();
  }

    break;

  case 141:

    {
    auto  left = unique_ptr<TensorValues<double>>((yystack_[4].value.TensorDoubleValues));
    auto right = unique_ptr<TensorValues<double>>((yystack_[1].value.TensorDoubleValues));

    string errorStr;
    if(!left->dimensionsMatch(*right, &errorStr)) {
      REPORT_ERROR(errorStr, yystack_[3].location);
    }
    left->merge(*right);
    (yylhs.value.TensorDoubleValues) = left.release();
  }

    break;

  case 142:

    {
    (yylhs.value.TensorDoubleValues) = (yystack_[0].value.TensorDoubleValues);
    (yylhs.value.TensorDoubleValues)->addDimension();
  }

    break;

  case 143:

    {
    auto  left = unique_ptr<TensorValues<double>>((yystack_[2].value.TensorDoubleValues));
    auto right = unique_ptr<TensorValues<double>>((yystack_[0].value.TensorDoubleValues));

    string errorStr;
    if(!left->dimensionsMatch(*right, &errorStr)) {
      REPORT_ERROR(errorStr, yystack_[1].location);
    }

    left->merge(*right);
    (yylhs.value.TensorDoubleValues) = left.release();
  }

    break;

  case 144:

    {
    (yylhs.value.TensorDoubleValues) = new TensorValues<double>();
    (yylhs.value.TensorDoubleValues)->addValue((yystack_[0].value.fnum));
  }

    break;

  case 145:

    {
    (yylhs.value.TensorDoubleValues) = (yystack_[2].value.TensorDoubleValues);
    (yylhs.value.TensorDoubleValues)->addValue((yystack_[0].value.fnum));
  }

    break;

  case 146:

    {
    // If the matrix has only one column then we discard that dimension and
    // treat it as a vector.
    if ((yystack_[0].value.TensorIntValues)->dimSizes[(yystack_[0].value.TensorIntValues)->dimSizes.size()-1] == 1) {
      (yystack_[0].value.TensorIntValues)->dimSizes.pop_back();
    }
  }

    break;

  case 148:

    {
    (yylhs.value.TensorIntValues) = (yystack_[1].value.TensorIntValues);
    (yylhs.value.TensorIntValues)->addDimension();
  }

    break;

  case 149:

    {
    auto  left = unique_ptr<TensorValues<int>>((yystack_[4].value.TensorIntValues));
    auto right = unique_ptr<TensorValues<int>>((yystack_[1].value.TensorIntValues));

    string errorStr;
    if(!left->dimensionsMatch(*right, &errorStr)) {
      REPORT_ERROR(errorStr, yystack_[3].location);
    }
    left->merge(*right);
    (yylhs.value.TensorIntValues) = left.release();
  }

    break;

  case 150:

    {
    (yylhs.value.TensorIntValues) = (yystack_[0].value.TensorIntValues);
    (yylhs.value.TensorIntValues)->addDimension();
  }

    break;

  case 151:

    {
    auto  left = unique_ptr<TensorValues<int>>((yystack_[2].value.TensorIntValues));
    auto right = unique_ptr<TensorValues<int>>((yystack_[0].value.TensorIntValues));

    string errorStr;
    if(!left->dimensionsMatch(*right, &errorStr)) {
      REPORT_ERROR(errorStr, yystack_[1].location);
    }

    left->merge(*right);
    (yylhs.value.TensorIntValues) = left.release();
  }

    break;

  case 152:

    {
    (yylhs.value.TensorIntValues) = new TensorValues<int>();
    (yylhs.value.TensorIntValues)->addValue((yystack_[0].value.num));
  }

    break;

  case 153:

    {
    (yylhs.value.TensorIntValues) = (yystack_[2].value.TensorIntValues);
    (yylhs.value.TensorIntValues)->addValue((yystack_[0].value.num));
  }

    break;

  case 154:

    {
    auto scalarType = new TensorType(ComponentType::INT);
    auto literal = new Literal(std::shared_ptr<TensorType>(scalarType), &(yystack_[0].value.num));
    (yylhs.value.TensorLiteral) = new shared_ptr<Literal>(literal);
  }

    break;

  case 155:

    {
    auto scalarType = new TensorType(ComponentType::FLOAT);
    auto literal = new Literal(std::shared_ptr<TensorType>(scalarType), &(yystack_[0].value.fnum));
    (yylhs.value.TensorLiteral) = new shared_ptr<Literal>(literal);
  }

    break;

  case 156:

    {
    auto call = shared_ptr<Call>(*(yystack_[3].value.call));
    auto expected = shared_ptr<Literal>(*(yystack_[1].value.TensorLiteral));
    delete (yystack_[3].value.call);
    delete (yystack_[1].value.TensorLiteral);

    std::vector<std::shared_ptr<Literal>> literalArgs;
    literalArgs.reserve(call->getArguments().size());
    for (auto &arg : call->getArguments()) {
      std::shared_ptr<Literal> litarg = dynamic_pointer_cast<Literal>(arg);
      if (!litarg) {
        REPORT_ERROR("function calls in tests must have literal arguments", yystack_[4].location);
      }
      literalArgs.push_back(litarg);
    }
    assert(literalArgs.size() == call->getArguments().size());

    std::vector<std::shared_ptr<Literal>> expecteds;
    expecteds.push_back(expected);
    (yylhs.value.test) = new Test(call->getName(), literalArgs, expecteds);
  }

    break;



            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
   Parser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
   Parser ::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short int  Parser ::yypact_ninf_ = -178;

  const signed char  Parser ::yytable_ninf_ = -100;

  const short int
   Parser ::yypact_[] =
  {
    -178,    16,  -178,  -178,  -178,    11,     6,    19,    50,    57,
     169,    42,   118,   169,    13,  -178,   169,  -178,  -178,  -178,
    -178,   120,  -178,   127,  -178,  -178,  -178,  -178,  -178,  -178,
     209,   111,   123,  -178,  -178,   140,   134,  -178,  -178,  -178,
    -178,    40,  -178,   142,  -178,  -178,  -178,   128,  -178,  -178,
     169,   173,  -178,   139,   144,   145,   163,   158,   229,   150,
    -178,   160,   146,   171,  -178,  -178,   133,   162,   154,   152,
     165,   167,   168,   155,   172,    27,   183,  -178,   197,  -178,
     169,   169,   169,  -178,  -178,  -178,   169,   169,  -178,  -178,
     169,  -178,   169,   169,   169,   169,   169,   169,   201,   169,
     204,     2,   169,   169,  -178,   229,   207,    85,  -178,    12,
     143,    60,  -178,   231,   169,   119,   169,    30,  -178,   -16,
      18,  -178,   208,   235,   236,  -178,   211,   239,   246,  -178,
     153,   237,   153,   187,   187,   229,    27,    27,    27,   249,
      63,    63,   187,   187,   229,  -178,   240,   230,  -178,    22,
     123,  -178,    46,    97,   248,  -178,   169,   250,  -178,  -178,
    -178,  -178,  -178,   228,  -178,  -178,   254,   283,  -178,  -178,
    -178,  -178,  -178,   285,  -178,   280,   141,   276,   267,  -178,
    -178,   235,   165,  -178,   239,   172,  -178,   284,    50,   286,
    -178,   169,  -178,  -178,   229,   143,    72,    30,   283,   264,
     109,  -178,   303,   292,  -178,   169,  -178,  -178,  -178,    31,
      64,  -178,   287,   277,  -178,  -178,    85,   273,   159,    21,
     275,   282,   314,  -178,   283,  -178,    77,  -178,   229,   153,
    -178,  -178,   291,    50,  -178,   290,  -178,  -178,  -178,     9,
    -178,  -178,   293,  -178,  -178,  -178,  -178,  -178,   153,   294,
    -178,  -178,   272,  -178,    21,   317,   318,  -178,  -178,  -178,
     110,   288,   126,  -178,  -178,   319,    60,  -178,   318,  -178,
    -178,  -178
  };

  const unsigned char
   Parser ::yydefact_[] =
  {
       2,    15,     1,   154,   155,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    44,     0,     3,     7,     4,
       5,     0,     6,     0,     8,    35,    36,    37,    38,    39,
       0,    50,    51,    53,    54,    55,     0,    56,    81,    57,
      58,     0,    97,   100,   101,    52,   132,   133,   135,     9,
      85,     0,    12,     0,     0,     0,     0,    59,    33,    51,
      41,     0,     0,     0,   152,   144,     0,     0,   139,   138,
     142,     0,   147,   146,   150,    61,     0,    33,     0,    33,
       0,     0,     0,    43,    68,    69,     0,     0,    70,    71,
       0,    65,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,     0,     0,   134,    87,     0,     0,   102,     0,
       0,     0,    10,     0,    85,    48,    85,     0,    60,     0,
       0,   136,     0,     0,     0,   137,     0,     0,     0,    18,
      16,     0,    20,    74,    75,    89,    63,    64,    66,    67,
      72,    73,    76,    77,    62,   103,     0,    86,    78,    99,
       0,    98,     0,     0,    82,   104,     0,     0,    11,    13,
     130,   131,   123,     0,   120,   110,     0,     0,    27,   106,
     107,   108,   109,    91,    34,     0,     0,     0,     0,   140,
     148,     0,   143,   145,     0,   151,   153,     0,    23,     0,
      83,     0,    42,   105,    88,     0,     0,     0,     0,   118,
       0,   116,     0,    93,    45,     0,    33,    84,   156,     0,
       0,    17,     0,    24,    25,    21,     0,     0,     0,     0,
       0,     0,     0,   115,     0,    92,     0,    90,    33,    47,
     141,   149,    28,     0,    14,     0,   127,   128,   129,     0,
     125,    40,   111,   119,   117,    95,    96,    94,    49,     0,
      22,    26,   121,   124,     0,     0,     0,   122,   126,   113,
       0,     0,     0,    30,   112,     0,     0,    29,     0,   114,
      32,    31
  };

  const short int
   Parser ::yypgoto_[] =
  {
    -178,  -178,  -178,  -178,  -178,  -178,  -178,  -178,  -178,  -178,
    -178,  -178,  -178,  -178,  -178,  -178,  -178,  -177,  -178,  -178,
      61,   -72,   329,  -178,  -178,  -178,  -178,  -178,  -178,  -178,
      -1,  -178,     0,  -178,  -178,  -178,  -178,  -178,  -178,   320,
     -76,   -48,  -178,  -178,  -178,  -178,  -178,  -178,   232,  -178,
    -178,    65,  -161,  -178,  -178,  -178,  -178,   112,   -97,  -178,
    -178,    80,   117,  -178,  -109,  -178,  -178,  -178,   -62,   214,
    -178,  -178,   -63,   212,  -178,  -178
  };

  const short int
   Parser ::yydefgoto_[] =
  {
      -1,     1,    17,    18,    19,   109,   159,    20,    21,   187,
      77,    22,    23,   189,    79,   212,   213,    55,   250,   262,
     263,   115,   174,    25,    26,    27,    28,    29,   175,   176,
     105,    31,    59,    33,    97,    34,    35,    36,    37,    38,
     106,   147,    39,    40,   203,   227,   247,    41,    42,    43,
      44,   168,   169,   170,   260,   171,   200,   201,   172,   196,
     239,   240,   164,    45,    46,    47,    67,    68,    69,    70,
      71,    72,    73,    74,    48,    49
  };

  const short int
   Parser ::yytable_[] =
  {
      30,    32,   107,   120,   119,   130,   199,   132,   178,    58,
     149,   214,    63,   163,    52,    75,     2,   179,    64,    65,
     157,     3,     4,   146,     5,   123,   236,    53,     6,   237,
       7,     8,    13,   -19,     9,     3,     4,   221,   158,    10,
     177,    50,   253,    11,    12,    66,    13,   254,    14,   -99,
      51,   180,   191,   -99,   152,   153,   251,    15,    54,   127,
      16,    51,    14,   199,   230,    56,   238,    82,   165,   160,
     161,   162,   123,   166,    88,    89,    90,    91,   101,   133,
     134,   135,   102,    60,   156,   136,   137,   192,   220,   138,
     167,   139,   140,   141,   142,   143,   144,   231,   217,    80,
      81,   150,   218,    82,   219,   127,    84,    85,    86,    87,
      88,    89,    90,    91,    30,    32,   155,    95,    96,   209,
     245,   210,   246,   156,     3,     4,    61,     5,   193,    30,
      32,    30,    32,     7,   229,   156,    76,     9,    64,    65,
     223,   264,    10,   216,    78,   -46,    11,   224,   265,    13,
     -79,    14,   160,   161,   162,   194,   248,   267,     3,     4,
      15,     5,    98,    16,   268,   205,   206,     7,   160,   161,
      99,     9,   103,   100,     3,     4,    10,    57,   104,   110,
      11,   108,   113,    13,   111,    14,   112,     9,   114,   -80,
     116,   129,   122,   123,    15,   121,   127,    16,   117,    13,
     125,    14,   118,   124,   228,   131,   126,    80,    81,   145,
     128,    82,   148,    16,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    82,    30,    32,
      84,    85,    86,    87,    88,    89,    90,    91,   154,   173,
     181,    65,   183,   184,    64,    80,    81,    30,    32,    82,
      83,   186,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    80,    81,   188,   156,    82,
     197,   190,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    80,    81,   -84,   198,    82,
     195,   165,    84,    85,    86,    87,    88,    89,    90,    91,
    -100,    93,    94,    95,    96,   202,   204,   207,   208,   222,
     211,   225,   215,   226,   234,   233,   241,   242,   232,   243,
     249,   252,   257,   255,   256,   259,   261,   269,   266,   271,
      24,   270,    62,   151,   258,   235,   244,   182,     0,   185
  };

  const short int
   Parser ::yycheck_[] =
  {
       1,     1,    50,    66,    66,    77,   167,    79,   117,    10,
       8,   188,    13,   110,     8,    16,     0,    33,     5,     6,
       8,     5,     6,    99,     8,    41,     5,     8,    12,     8,
      14,    15,    30,    17,    18,     5,     6,   198,    26,    23,
     116,    30,    33,    27,    28,    32,    30,    38,    32,    38,
      39,    33,    30,    42,   102,   103,   233,    41,     8,    41,
      44,    39,    32,   224,    33,     8,    45,    40,     8,     9,
      10,    11,    41,    13,    47,    48,    49,    50,    38,    80,
      81,    82,    42,    41,    38,    86,    87,    41,   197,    90,
      30,    92,    93,    94,    95,    96,    97,    33,   195,    36,
      37,   101,    30,    40,    32,    41,    43,    44,    45,    46,
      47,    48,    49,    50,   115,   115,    31,    54,    55,   181,
      43,   184,    45,    38,     5,     6,     8,     8,    31,   130,
     130,   132,   132,    14,   206,    38,    16,    18,     5,     6,
      31,    31,    23,   191,    17,    26,    27,    38,    38,    30,
      39,    32,     9,    10,    11,   156,   228,    31,     5,     6,
      41,     8,    39,    44,    38,    24,    25,    14,     9,    10,
      30,    18,    30,    39,     5,     6,    23,     8,    50,    40,
      27,     8,    19,    30,    40,    32,    41,    18,    30,    39,
      30,     8,    38,    41,    41,    33,    41,    44,    52,    30,
      33,    32,    31,    38,   205,     8,    38,    36,    37,     8,
      38,    40,     8,    44,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    40,   229,   229,
      43,    44,    45,    46,    47,    48,    49,    50,    31,     8,
      32,     6,     6,    32,     5,    36,    37,   248,   248,    40,
      41,     5,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    36,    37,    30,    38,    40,
      42,    31,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    36,    37,    39,    34,    40,
      40,     8,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    20,    26,    31,    41,    45,
      26,     8,    26,    21,    41,    38,    41,    35,    31,     5,
      29,    31,    50,    30,    30,     8,     8,     8,    40,   268,
       1,   266,    12,   101,   254,   218,   224,   123,    -1,   127
  };

  const unsigned char
   Parser ::yystos_[] =
  {
       0,    57,     0,     5,     6,     8,    12,    14,    15,    18,
      23,    27,    28,    30,    32,    41,    44,    58,    59,    60,
      63,    64,    67,    68,    78,    79,    80,    81,    82,    83,
      86,    87,    88,    89,    91,    92,    93,    94,    95,    98,
      99,   103,   104,   105,   106,   119,   120,   121,   130,   131,
      30,    39,     8,     8,     8,    73,     8,     8,    86,    88,
      41,     8,    95,    86,     5,     6,    32,   122,   123,   124,
     125,   126,   127,   128,   129,    86,    16,    66,    17,    70,
      36,    37,    40,    41,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    90,    39,    30,
      39,    38,    42,    30,    50,    86,    96,    97,     8,    61,
      40,    40,    41,    19,    30,    77,    30,    52,    31,   124,
     128,    33,    38,    41,    38,    33,    38,    41,    38,     8,
      77,     8,    77,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,     8,    96,    97,     8,     8,
      88,   104,    97,    97,    31,    31,    38,     8,    26,    62,
       9,    10,    11,   114,   118,     8,    13,    30,   107,   108,
     109,   111,   114,     8,    78,    84,    85,    96,   120,    33,
      33,    32,   125,     6,    32,   129,     5,    65,    30,    69,
      31,    30,    41,    31,    86,    40,   115,    42,    34,   108,
     112,   113,    20,   100,    26,    24,    25,    31,    41,   124,
     128,    26,    71,    72,    73,    26,    97,   114,    30,    32,
     120,   108,    45,    31,    38,     8,    21,   101,    86,    77,
      33,    33,    31,    38,    41,   118,     5,     8,    45,   116,
     117,    41,    35,     5,   113,    43,    45,   102,    77,    29,
      74,    73,    31,    33,    38,    30,    30,    50,   117,     8,
     110,     8,    75,    76,    31,    38,    40,    31,    38,     8,
     107,    76
  };

  const unsigned char
   Parser ::yyr1_[] =
  {
       0,    56,    57,    57,    58,    58,    58,    58,    58,    58,
      59,    60,    61,    61,    62,    64,    65,    63,    66,    68,
      69,    67,    70,    71,    71,    72,    72,    73,    74,    74,
      75,    75,    76,    77,    77,    78,    78,    78,    78,    78,
      79,    80,    81,    82,    82,    83,    84,    84,    85,    85,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    87,
      88,    89,    89,    89,    89,    89,    89,    89,    90,    90,
      90,    90,    91,    91,    91,    91,    91,    91,    92,    93,
      93,    93,    94,    94,    95,    96,    96,    97,    97,    98,
      99,   100,   100,   101,   101,   102,   102,   103,   103,   104,
     104,   104,   105,   105,   106,   106,   107,   107,   107,   107,
     108,   109,   109,   110,   110,   111,   112,   112,   113,   113,
     114,   114,   114,   115,   115,   116,   116,   117,   117,   117,
     118,   118,   119,   120,   120,   120,   121,   121,   122,   122,
     123,   123,   124,   124,   125,   125,   126,   126,   127,   127,
     128,   128,   129,   129,   130,   130,   131
  };

  const unsigned char
   Parser ::yyr2_[] =
  {
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       3,     4,     0,     2,     4,     0,     0,     5,     2,     0,
       0,     5,     6,     0,     1,     1,     3,     3,     0,     4,
       1,     3,     3,     0,     2,     1,     1,     1,     1,     1,
       7,     2,     4,     2,     1,     5,     0,     3,     0,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     3,     3,     3,     2,     3,     3,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     4,     4,     4,     0,     1,     1,     3,     3,
       6,     0,     2,     0,     2,     1,     1,     1,     3,     1,
       1,     1,     3,     3,     4,     4,     1,     1,     1,     1,
       1,     4,     7,     1,     3,     3,     1,     3,     1,     3,
       1,     5,     6,     0,     4,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     1,     1,
       3,     5,     1,     3,     1,     3,     1,     1,     3,     5,
       1,     3,     1,     3,     1,     1,     5
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  Parser ::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "UNKNOWN", "NEG",
  "\"int literal\"", "\"float literal\"", "\"string literal\"",
  "\"identifier\"", "\"int\"", "\"float\"", "\"tensor\"", "\"element\"",
  "\"set\"", "\"const\"", "\"extern\"", "\"proc\"", "\"func\"", "\"map\"",
  "\"to\"", "\"with\"", "\"reduce\"", "\"while\"", "\"if\"", "\"elif\"",
  "\"else\"", "\"end\"", "\"return\"", "\"%!\"", "\"->\"", "\"(\"",
  "\")\"", "\"[\"", "\"]\"", "\"{\"", "\"}\"", "\"<\"", "\">\"", "\",\"",
  "\".\"", "\":\"", "\";\"", "\"=\"", "\"+\"", "\"-\"", "\"*\"", "\"/\"",
  "\".*\"", "\"./\"", "\"^\"", "\"'\"", "\"\\\\\"", "\"==\"", "\"!=\"",
  "\"<=\"", "\">=\"", "$accept", "program", "program_element", "extern",
  "element_type_decl", "field_decl_list", "field_decl", "procedure", "$@1",
  "$@2", "procedure_header", "function", "$@3", "$@4", "function_header",
  "arguments", "argument_list", "argument_decl", "results", "result_list",
  "result_decl", "stmt_block", "stmt", "const_stmt", "return_stmt",
  "assign_stmt", "expr_stmt", "if_stmt", "else_clauses", "elif_clauses",
  "expr", "ident_expr", "paren_expr", "linear_algebra_expr",
  "elwise_binary_op", "boolean_expr", "field_read_expr", "set_read_expr",
  "call_or_tensor_read_expr", "call_expr", "expr_list_or_empty",
  "expr_list", "range_expr", "map_expr", "with", "reduce", "reduction_op",
  "write_expr_list", "write_expr", "field_write_expr", "tensor_write_expr",
  "type", "element_type", "set_type", "endpoints", "tuple_type",
  "tuple_element_types", "tuple_element_type", "tensor_type",
  "nested_dimensions", "dimensions", "dimension", "component_type",
  "literal_expr", "tensor_literal", "dense_tensor_literal",
  "float_dense_tensor_literal", "float_dense_ndtensor_literal",
  "float_dense_matrix_literal", "float_dense_vector_literal",
  "int_dense_tensor_literal", "int_dense_ndtensor_literal",
  "int_dense_matrix_literal", "int_dense_vector_literal", "scalar_literal",
  "test", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
   Parser ::yyrline_[] =
  {
       0,   248,   248,   250,   254,   255,   263,   271,   274,   278,
     285,   299,   313,   316,   324,   338,   338,   338,   346,   368,
     368,   368,   376,   409,   412,   418,   423,   431,   440,   443,
     449,   454,   462,   472,   475,   484,   485,   486,   487,   488,
     492,   522,   528,   605,   608,   614,   620,   622,   626,   628,
     642,   643,   644,   645,   646,   647,   648,   649,   650,   656,
     677,   693,   701,   713,   778,   784,   814,   819,   828,   829,
     830,   831,   837,   843,   849,   855,   861,   867,   882,   902,
     903,   904,   915,   937,   943,   953,   956,   962,   968,   979,
     990,   998,  1000,  1004,  1006,  1009,  1010,  1058,  1063,  1071,
    1075,  1078,  1084,  1102,  1108,  1126,  1156,  1159,  1162,  1165,
    1171,  1178,  1182,  1192,  1196,  1203,  1210,  1213,  1221,  1226,
    1242,  1245,  1250,  1258,  1261,  1299,  1304,  1312,  1315,  1319,
    1324,  1327,  1396,  1399,  1400,  1404,  1407,  1416,  1427,  1434,
    1437,  1441,  1454,  1458,  1472,  1476,  1482,  1489,  1492,  1496,
    1509,  1513,  1527,  1531,  1537,  1542,  1552
  };

  // Print the state stack on the debug stream.
  void
   Parser ::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
   Parser ::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
   Parser ::token_number_type
   Parser ::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
    };
    const unsigned int user_token_number_max_ = 310;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} } //  simit::internal 



