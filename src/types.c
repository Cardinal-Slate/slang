/* src/types.c — the type layer as three spine ops. Slate-only.
   SPDX-License-Identifier: MIT OR Apache-2.0

   A type is a structure, told apart by its tag. cse_type_has and cse_type_fits are slate_order (level =
   the same type); cse_type_derive is slate_compose over a function type (payload = in, rest = out). There
   is no type checker here beyond the spine — order decides membership and fit, compose builds. */
#include "cse/types.h"
#include "cse/dsa/cell.h"

/* v : t — v has type t when they order as level. */
slate_psda *cse_type_has(slate_psda *v, slate_psda *t) {
  return (slate_order(v, t).potential == slate_level()) ? v : 0;
}

/* a function type in → out — a cell carrying in, keyed to out. */
slate_psda *cse_type_fn(slate_psda *spare, slate_psda *in, slate_psda *out) {
  return cse_cell(spare, in, out, 0);   /* payload = in, rest = out */
}

/* v fits fn — v orders level with fn's input. */
slate_psda *cse_type_fits(slate_psda *v, slate_psda *fn) {
  return (slate_order(v, cse_cell_payload(fn)).potential == slate_level()) ? v : 0;
}

/* f ∘ g — if out(f) and in(g) order as level, compose into in(f) → out(g); else nothing. */
slate_psda *cse_type_derive(slate_psda *spare, slate_psda *f, slate_psda *g) {
  return (slate_order(cse_cell_rest(f), cse_cell_payload(g)).potential == slate_level())
       ? cse_cell(spare, cse_cell_payload(f), cse_cell_rest(g), 0)
       : 0;
}
