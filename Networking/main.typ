#import "template.typ": *

#show: project.with(
  title: "Networking",
  authors: (
    (name: "Salvatore Andaloro"),
  ),
)

// We generated the example code below so you can see how
// your document will look. Go ahead and replace it with
// your own content!
#outline(
  indent: auto,
  depth: 3
)
#pagebreak()

#include "00 Introduction.typ"
#pagebreak()
#include "01 Application layer.typ"
#pagebreak()
#include "02 Transport layer.typ"
#pagebreak()
#include "03 Network layer.typ"
#pagebreak()
#include "04 Routing protocols.typ"
#pagebreak()
#include "05 Data link layer.typ"