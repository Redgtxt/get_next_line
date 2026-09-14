# get_next_line

![42 School](https://img.shields.io/badge/42-Lisboa-000000?style=flat-square&logo=42&logoColor=white)
![Milestone](https://img.shields.io/badge/milestone-2-informational?style=flat-square)
![Language](https://img.shields.io/badge/language-C-00599C?style=flat-square&logo=c&logoColor=white)
![Norm](https://img.shields.io/badge/norminette-passing-success?style=flat-square)
![Flags](https://img.shields.io/badge/flags-Wall%20%7C%20Wextra%20%7C%20Werror-critical?style=flat-square)

> Read a file descriptor one line at a time — with a buffer that survives between calls, and no idea in advance how long a line is.

```c
char *get_next_line(int fd);
```

One function, one prototype, and a problem that is much larger than it looks. `read` gives you a fixed-size chunk of bytes; a line ends wherever it ends. So every call has to deal with a chunk containing no newline (keep reading), a chunk containing one in the middle (return the first half, *remember the second*), and a chunk containing several. The remembered part is the whole project: it is what forces static storage, and what makes this the first 42 project where a function has memory of its own past.

> **Note** — the same code also ships inside my [Libft](https://github.com/Redgtxt/Libft), so that later projects only need to link one archive. This repository is the standalone Milestone 2 deliverable.

---

## Table of contents

- [What's inside](#whats-inside)
- [Building](#building)
- [Behaviour](#behaviour)
- [How it works](#how-it-works)
- [Design notes](#design-notes)
- [What I took away from it](#what-i-took-away-from-it)

---

## What's inside

```
get_next_line/
└── get_next_line/
    ├── get_next_line.h        # prototype + BUFFER_SIZE default
    ├── get_next_line.c        # the read loop
    └── get_next_line_utils.c  # ft_strlen, ft_strjoin, flagfunc
```

Three helpers, each bent to this specific job rather than borrowed from `libft`:

| Helper | What it does here |
| --- | --- |
| `ft_strlen` | Length **up to and including the first `\n`**, not up to `\0` |
| `ft_strjoin` | Appends, stops at a newline, and frees its left operand |
| `flagfunc` | Consumes the buffer up to the newline and shifts the leftover to the front |

## Building

There is no `Makefile` here — the project is compiled directly into whatever uses it, with `BUFFER_SIZE` chosen at compile time:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o demo
```

The header falls back to `BUFFER_SIZE 1` when the flag is absent, which is deliberately the worst case: if it works at 1 byte per `read`, the buffer logic is right.

```c
#include "get_next_line.h"

int main(void)
{
    int   fd   = open("file.txt", O_RDONLY);
    char *line = get_next_line(fd);

    while (line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (0);
}
```

## Behaviour

- Returns the next line **including its trailing `\n`**, or the final line without one if the file does not end in a newline.
- Returns `NULL` when there is nothing left to read.
- The returned string is heap-allocated and **owned by the caller** — every line must be freed.
- `BUFFER_SIZE` is independent of line length: a 1-byte buffer and a 10 KB buffer produce identical output.

## How it works

```
          ┌──────────────────────────── static buffer (survives between calls)
          │
  ┌───────▼────────┐   no newline yet   ┌──────────────┐
  │ leftover bytes │ ──────────────────►│ read() more  │
  └───────┬────────┘                    └──────┬───────┘
          │                                    │
          │  ft_strjoin: append up to '\n'     │
          ▼                                    │
  ┌────────────────┐                           │
  │ growing line   │◄──────────────────────────┘
  └───────┬────────┘
          │  flagfunc: newline found?
          │       yes ──► erase the consumed part, shift the rest to the front, return the line
          └────── no ───► loop
```

The loop condition is the compact statement of all of it:

```c
while (buffer[0] || (read(fd, buffer, BUFFER_SIZE)) > 0)
```

Leftover from the previous call is consumed **before** touching `read` again. Only when the buffer is empty does a new `read` happen.

Two details carry the design:

- **`ft_strlen` stops at `\n`.** Because it reports the length only as far as the newline, `ft_strjoin` allocates exactly the bytes belonging to the current line and copies no further — the split happens implicitly during the concatenation rather than as a separate scan-and-cut step.
- **`flagfunc` is destructive on purpose.** It walks the buffer zeroing each byte as it passes, and once it has crossed the newline it starts writing the remaining bytes back at index 0. In a single pass it both clears what was already returned and repositions what is still owed to the next call. Its return value doubles as the "was there a newline?" answer that ends the loop.

## Design notes

- **The static lives inside `get_next_line`, not at file scope.** It is state, but it is nobody else's business — the narrowest scope that outlives the call.
- **`ft_strjoin` frees `s1`.** A normal `strjoin` returning a fresh string would leak one allocation per iteration of the loop. Folding the free into the join means the growing line always has exactly one owner, at the cost of a function that is *not* a drop-in for `libft`'s version. Hence the separate file rather than reusing `libft`.
- **Erase-and-shift instead of an index.** Keeping a static offset into the buffer would be the usual approach; compacting the buffer instead means the only state is the buffer itself, and `buffer[0]` alone answers "is there anything pending?".
- **Scope.** This is the mandatory part: one static buffer, so one file descriptor at a time. The bonus — a single static tracking several descriptors simultaneously — is not implemented.

## What I took away from it

- Static variables as function-local state, and the price it carries: a function with memory is a function that is no longer reentrant.
- That `read` returning fewer bytes than requested is normal, not an error, and that `0` and `-1` mean very different things.
- Ownership discipline around `malloc`: in a loop that reallocates on every iteration, "who frees this" has to be answered once and then held to.
- Testing the edge cases that actually break line readers — an empty file, a file with no trailing newline, consecutive newlines, `BUFFER_SIZE` of 1, a line longer than the buffer by a single byte.

---

**Author** — Hugo Pinto ([`hguerrei`](https://profile.intra.42.fr/users/hguerrei) · [@Redgtxt](https://github.com/Redgtxt))
