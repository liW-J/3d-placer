ADD_LIBRARY (PlaceDB placedb.cpp placedb_bookshelf.cpp SteinerDecomposition.cpp)
INCLUDE_DIRECTORIES (
	${PROJECT_SOURCE_DIR}/GlobalRouting
	${PROJECT_SOURCE_DIR}/LibDB
	${PROJECT_SOURCE_DIR}/NLPlace
	${PROJECT_SOURCE_DIR}/Parser
	${PROJECT_SOURCE_DIR}/PlaceCommon
	${PROJECT_SOURCE_DIR}/PlaceDB
	${PROJECT_SOURCE_DIR}/XSTree
)
ADD_DEFINITIONS (-D__PLOTTER_BIN__="")
