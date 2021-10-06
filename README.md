# X Enhanced Regex Project (XRegex)

This regex parser contains support for named submatches, both locally within the regular expression and globally through other named regular expressions.
The goal is for this regular expression parser to be more intuitive, and designed more for language generation than traditional regular expressions.

Additionally, the support for named regular expression types eliminates the need for popular shortcuts like `\s`.
Instead, the user can simply define:

```text
WHITESPACE = "[ \t\r\n]"
```

And then use `${WHITESPACE}` in place of `\s`.
This addition also includes some other adjustments, detailed in the "Proposed Adjustments" section.

## Proposed Adjustments

- Demote the submatch operator `()` so that it can no longer be accessed directly using numbered switches (e.g. `\1` or `\2`).
  This will simplify the accessing of regular expression submatches to named entities only.

- Introduce the *expression import* operator (`${NAME}`), which will behave as an unmarked submatch where the expression is imported from `NAME`.
  For example, if we had the named entity `DIGIT` defined as `[0-9]`, then the expression for matching a `DOUBLE` would be `${DIGIT}+\.${DIGIT}+`.
  This import statement will work with both **global** and **local** named entities.

- Introduce the *named submatch* definition operator, defined as `$(NAME:VALUE)`.
  This operator will create a local definition for a submatch corresponding to the given name.
  This submatch will be available for use within the target expression only.

- Introduce the `explicit submatch copy` operator.
  This replaces functionality lost by eliminating the numbered switches corresponding to each submatch.
  This syntax requires that the input **explicitly** match the value obtained from the **first** instance of the named submatch.
  For example, if given the regular expression `$(LETTER:[a-z])+` and the input `bad`, then the only character which would match `$(LETTER)` is `b`.
  Note that this **only** works on local entities.
  If you want to use explicit matching on a gloabal entity, then you must wrap it in a local expression (for example, `$(LOCAL:${GLOBAL})`).

- Adjust the way multimatch expressions are handled.
  This will allow the user to write multimatch expressions on an inclusion-exclusion basis without removing any existing functionality.
  For example, the multimatch expression `[a-z^b]` would match every character from `a` to `z` **except** for `b`.
  If an exclusion clause is present with no inclusion, all letters are assumed to be included.
  If no clauses are supplied, then the statement is invalid.
  Otherwise, the only included characters are assumed to be those provided by the inclusion statement.
  If a character is excluded but never included, then it is excluded.
