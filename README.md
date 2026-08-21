# slang

The type layer for Cardinal-Slate. **A type is a structure**, the same substance as a value; typing and
fit are `order`; deriving a composite type is `compose`. No schema, no slots — the spine's three ops are
the whole type checker.

## Role

**Depends on the spine (CardinalSlate) and [CSE-DSA](https://github.com/Cardinal-Slate/CSE-DSA)** (to hold
a function type as a cell) — nothing else. It sits in the structural band, right on the spine, so
everything above it (programs, providers, an API contract) can carry and check its own type.

- `slang_has(v, t)` — `v : t`, i.e. `slate_order(v, t)` is `level`. A value *has* a type by sitting level
  with it; distinct tags don't, so a `Point` is not a `Vector` even with the same numbers.
- `slang_fn(spare, in, out)` — a function type `in → out` (a cell: payload = in, rest = out).
- `slang_fits(v, fn)` — `v` can be passed where `fn` expects its input (`order` level).
- `slang_derive(spare, f, g)` — `f ∘ g`'s type: if `out(f)` and `in(g)` are `level`, `compose` into
  `in(f) → out(g)`; else nothing.

That's the whole thing. There is no type-checker to write — `order` checks membership and fit, `compose`
derives, `decompose` reads. The type system is the spine looking at itself.

```
# from the tests — Point and Vector, one order
Vector{3,4} : Vector          # order level  → yes
Point{3,4}  : Vector          # different tag → no
length : Vector → Scalar      # a Vector fits; a Point does not
direction : Point → Vector  ∘  length : Vector → Scalar  =  distance : Point → Scalar
```

## Build

```
make            gate + standalone header + Point/Vector test, then libslang.a
make check      the gate and the test only
```

Build the deps first: `make -C ../CardinalSlate lib && make -C ../CSE-DSA lib`.

## License

MIT OR Apache-2.0.
