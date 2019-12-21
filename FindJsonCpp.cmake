FIND_PATH(JSONCPP_INCLUDE_DIR json/json.h PATH_SUFFIXES include/jsoncpp jsoncpp)
FIND_LIBRARY(JSONCPP_LIBRARY_DEBUG jsoncpp)
FIND_LIBRARY(JSONCPP_LIBRARY_RELEASE jsoncpp)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(JsonCpp DEFAULT_MSG JSONCPP_LIBRARY_DEBUG JSONCPP_LIBRARY_RELEASE JSONCPP_INCLUDE_DIR)

MARK_AS_ADVANCED(
  JSONCPP_INCLUDE_DIR
  JSONCPP_LIBRARY_DEBUG
  JSONCPP_LIBRARY_RELEASE
)
