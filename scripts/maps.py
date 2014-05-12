"""Contains useful mappings from raw strings to presentable
strings to use for graphs and tables."""

STRUCTURE_NAME_MAP = {
	"sequential_scan" : "Sequential Scan",
	"octree" : "Octree",
	"pseudo_pyramid_tree" : "Pseudo-Pyramid Tree",
	"pyramid_tree" : "Pyramid Tree",
	"dht" : "Bit Hash",
	"kdtree" : "KD-Tree",
	"multigrid" : "Multigrid",
	
	"index_pseudo_pyramid_tree" : "Rebuild Index PPT",
	"splay_pseudo_pyramid_tree" : "Splay PPT"
}

OPERATION_NAME_MAP = {
	"insert" : "Insert",
	"delete" : "Delete",
	"pquery" : "Point Query"
}

COLOURS = [ 'b', 'c', 'y', 'm', 'r', 'g' ]
MARKERS = [ 'o', 'v', 's', 'D', 'x', '^' ]
PATTERNS = [ "/" , "\\" , "|" , "-" , "+" , "x", "o", "O", ".", "*" ]
