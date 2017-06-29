# EECS3301-Project

- parse tokens, construct a link list of tokens

- Token structure
```
struct {
    int type; // defined by #define section
    int * value;

    token * nextToken;
} token;
```

- LR compute the result

