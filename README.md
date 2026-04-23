*This project has been created as part of the 42 curriculum by egonin, ltourbe.*

## Description
`minishell` is a small shell implemented in C. It reads user input, tokenizes then parses it, performs basic expansions, and executes commands.

Main features:
- Command execution via `PATH` resolution
- Pipelines (`|`)
- Redirections: input (`<`), output (`>`), append (`>>`), heredoc (`<<`)
- Environment management and builtins (`cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`)

## Instructions

### Build
From the repository root: `make` produces the `minishell` executable.

### Run
```sh
./minishell
```

### Valgrind
Recommended commands used for this project:

- Full leak check (non-interactive, ends with EOF):
```sh
valgrind --leak-check=full --show-leak-kinds=all --child-silent-after-fork=yes ./minishell << 'EOF'
echo hello | cat
cat << EOM
abc
EOM
EOF
```

- File descriptor check:
```sh
valgrind --track-fds=yes --leak-check=no --child-silent-after-fork=yes ./minishell << 'EOF'
echo hello | cat
cat << EOM
abc
EOM
EOF
```

Notes:
- Prefer EOF input instead of typing `exit` when checking leaks, to avoid extra noise in reports.
- `still reachable` blocks from `readline`/`ncurses` are expected.

### Tester (optional)
A test suite is available here : https://github.com/LucasKuhn/minishell_tester 

## Resources
### References
- https://medium.com/@zouhairlrs/minishell-building-a-mini-bash-a-42-project-5dc20d671fbb
- https://www.reddit.com/r/explainlikeimfive/comments/1vu6g7/eli5_shell_computing/

### AI usage
AI was used to draft an agenda and to split the workload efficently between us two. We used it as well to ensure there were no leaks.