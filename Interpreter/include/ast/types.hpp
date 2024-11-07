#pragma

enum class NodeType {
    PROGRAM,
    /* Routines, conditional or loop block */
    BLOCK_PROGRAM,
    /* Values */
    NUMBER_LITERAL,
    STRING_LITERAL,
    // NULL_LITERAL,
    BOOL,
    RESERVED_LITERAL,
    /* Identifier */
    IDENTIFIER,
    /* Statement */
    VAR_DECLARATION,
    ASSIGNMENT,
    CONDITIONAL,
    /* LOGIC OPERATORS */
    LOGIC_BINARY,
    NOT,
    /* Functions Declarations */
    FUNCTION_DECLARATION,
    RETURN,
    FVAR_DECLARATION,
    /* Others */
    RESERVED,
    BINARY_EXPR,
    CALL_EXPR,
    UNARY_EXPR,
    EXPRESSION,
    UNKNOW,
};
