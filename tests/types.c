/* tests/types.c — Point and Vector: typing and fit are order, deriving is compose.
   SPDX-License-Identifier: MIT OR Apache-2.0 */
#include <stdio.h>
#include "cse/types.h"
#include "cse/dsa/cell.h"
#include "slate/psda.h"
#include "slate/encode.h"

static int fails = 0;
#define CHECK(c, m) do { if (!(c)) { printf("  FAIL %s\n", (m)); fails++; } } while (0)

/* a value/type of a given tag — a psda whose reading is the tag id (the tag is what tells types apart) */
static unsigned char tbuf[24][4];
static slate_psda tnodes[24];
static int tn = 0;
static slate_psda *tagged(unsigned id) {
  unsigned char b = (unsigned char)id;
  slate_encode(&b, 1, tbuf[tn]);
  tnodes[tn].potential = tbuf[tn]; tnodes[tn].prev = 0; tnodes[tn].next = 0;
  return &tnodes[tn++];
}
enum { VECTOR = 1, POINT = 2, SCALAR = 3 };

/* a blank cell for a function type */
static slate_psda spares[8];
static int sn = 0;
static slate_psda *spare(void) { spares[sn].potential = 0; spares[sn].prev = 0; spares[sn].next = 0; return &spares[sn++]; }

int main(void) {
  /* ② v : T is order — a Vector value has type Vector; a Point does not, same numbers or not */
  CHECK(cse_type_has(tagged(VECTOR), tagged(VECTOR)) != 0, "Vector{3,4} : Vector");
  CHECK(cse_type_has(tagged(POINT),  tagged(VECTOR)) == 0, "Point{3,4} is NOT a Vector");

  /* ③ fit is order — length : Vector → Scalar accepts a Vector, rejects a Point */
  slate_psda *length = cse_type_fn(spare(), tagged(VECTOR), tagged(SCALAR));
  CHECK(cse_type_fits(tagged(VECTOR), length) != 0, "a Vector fits length");
  CHECK(cse_type_fits(tagged(POINT),  length) == 0, "a Point does not fit length");

  /* ④ deriving is compose — direction : Point → Vector  ∘  length : Vector → Scalar  =  distance */
  slate_psda *direction = cse_type_fn(spare(), tagged(POINT),  tagged(VECTOR));
  slate_psda *length2   = cse_type_fn(spare(), tagged(VECTOR), tagged(SCALAR));
  slate_psda *distance  = cse_type_derive(spare(), direction, length2);
  CHECK(distance != 0, "direction and length fit at Vector → compose");
  CHECK(cse_type_has(cse_cell_payload(distance), tagged(POINT))  != 0, "distance's input is Point");
  CHECK(cse_type_has(cse_cell_rest(distance),    tagged(SCALAR)) != 0, "distance's output is Scalar");

  /* the wrong order does not fit: length ∘ direction (Scalar ≠ Point) */
  CHECK(cse_type_derive(spare(), length2, direction) == 0, "length ∘ direction does NOT fit");

  printf(fails ? "types: FAIL\n" : "types: ok\n");
  return fails ? 1 : 0;
}
