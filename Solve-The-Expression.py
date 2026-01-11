def read_pattern(rows, count):
    patterns = []
    for i in range(count):
        pattern = []
        for row in rows:
            pattern.append(row[i * 3 : i * 3 + 3])
        patterns.append("".join(pattern))
    return patterns


def pattern_to_binary(pattern):
    return "".join(["1" if c != " " else "0" for c in pattern])


def solve():
    digits_rows = [input() for _ in range(3)]
    op_rows = [input() for _ in range(3)]
    expr_rows = [input() for _ in range(3)]

    digit_patterns = read_pattern(digits_rows, 10)
    ops_patterns = read_pattern(op_rows, 5)

    digits_map = {}
    for i, pat in enumerate(digit_patterns):
        digits_map[pattern_to_binary(pat)] = str(i)

    ops = ["||", "&&", "!", "(", ")"]
    ops_map = {pattern_to_binary(ops_patterns[i]): ops[i] for i in range(5)}

    total_cols = len(expr_rows[0])
    expr_count = total_cols // 3
    expr_patterns = read_pattern(expr_rows, expr_count)

    expression = ""
    for pat in expr_patterns:
        bpat = pattern_to_binary(pat)
        if bpat in digits_map:
            expression += digits_map[bpat]
        elif bpat in ops_map:
            expression += " " + ops_map[bpat] + " "
        else:
            pass  # ignore unknown symbols

    expression = expression.strip()

    # Evaluate bitwise logic
    def bitwise_and(a, b):
        la, lb = len(a), len(b)
        m = max(la, lb)
        a, b = a.zfill(m), b.zfill(m)
        return "".join("1" if a[i] == "1" and b[i] == "1" else "0" for i in range(m))

    def bitwise_or(a, b):
        la, lb = len(a), len(b)
        m = max(la, lb)
        a, b = a.zfill(m), b.zfill(m)
        return "".join("1" if a[i] == "1" or b[i] == "1" else "0" for i in range(m))

    def bitwise_not(a):
        return "".join("1" if c == "0" else "0" for c in a)

    def to_binary(num):
        s = str(num)
        return "".join(pattern_to_binary(digit_patterns[int(ch)]) for ch in s)

    def from_binary(bin_str):
        n = len(bin_str)
        for num in range(1000):
            if to_binary(num).zfill(n) == bin_str.zfill(n):
                return num
        return 0

    def eval_expr(expr):
        tokens = expr.replace("(", " ( ").replace(")", " ) ").split()
        stack_val, stack_op = [], []

        def apply():
            op = stack_op.pop()
            if op == "!":
                a = stack_val.pop()
                stack_val.append(bitwise_not(a))
            else:
                b, a = stack_val.pop(), stack_val.pop()
                stack_val.append(bitwise_and(a, b) if op == "&&" else bitwise_or(a, b))

        prec = {"!": 3, "||": 2, "&&": 1}

        for token in tokens:
            if token.isdigit():
                stack_val.append(to_binary(int(token)))
            elif token == "(":
                stack_op.append(token)
            elif token == ")":
                while stack_op and stack_op[-1] != "(":
                    apply()
                stack_op.pop()
            else:
                while stack_op and stack_op[-1] != "(" and prec.get(stack_op[-1], 0) >= prec.get(token, 0):
                    apply()
                stack_op.append(token)

        while stack_op:
            apply()

        return from_binary(stack_val[-1])

    print(eval_expr(expression))


# Run the solver
solve()