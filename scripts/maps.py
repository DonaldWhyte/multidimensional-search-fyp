"""Contains useful mappings from raw strings to presentable
strings to use for graphs and tables."""

STRUCTURE_NAME_MAP = {
	"sequential_scan" : "Sequential Scan",
	"octree" : "Octree",
	"index_pseudo_pyramid_tree" : "Rebuild Index PPT",
	"pseudo_pyramid_tree" : "Bucket PPT",
	"splay_pseudo_pyramid_tree" : "Splay PPT",
	"kdtree" : "KD-Tree",
	"dht" : "Bucket Hash Table",
	"pyramid_tree" : "Pyramid Tree"
}

OPERATION_NAME_MAP = {
	"insert" : "Insert",
	"delete" : "Delete",
	"pquery" : "Point Query"
}
