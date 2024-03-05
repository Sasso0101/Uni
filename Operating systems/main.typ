#import "template.typ": *

#show: project.with(
  title: "Operating systems",
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
#include "01 Processes.typ"
#pagebreak()