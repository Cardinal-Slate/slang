/// cse/types.h — the type layer: a type is a structure, typing is order, deriving is compose.
/// SPDX-License-Identifier: MIT OR Apache-2.0
///
/// No schema, no slots. A type is a psda — the same substance as a value — told apart by its tag. Having
/// a type, and fitting, are `slate_order` (level = the same type); deriving a composite type is
/// `slate_compose` over a function type (payload = in, rest = out). The three spine ops are the whole
/// type checker; this layer only names the three roles. Only slate measurements cross. It sits directly
/// on the spine (and CSE-DSA to hold a function type), so everything above can carry and check its type.
#pragma once
#include "slate/psda.h"

/// v : t — v has type t (they order as level). Returns v, or nothing.
slate_psda *cse_type_has(slate_psda *v, slate_psda *t);

/// a function type `in → out` — a structure (payload = in, rest = out). `spare` is a blank cell.
slate_psda *cse_type_fn(slate_psda *spare, slate_psda *in, slate_psda *out);

/// v fits fn — v can be passed where fn expects its input (order level). Returns v, or nothing.
slate_psda *cse_type_fits(slate_psda *v, slate_psda *fn);

/// f ∘ g — the composite type in(f) → out(g), when out(f) and in(g) order as level. `spare` is a blank
/// cell. Returns the composite type, or nothing if they do not fit.
slate_psda *cse_type_derive(slate_psda *spare, slate_psda *f, slate_psda *g);
