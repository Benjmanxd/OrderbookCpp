file(GLOB ORDERBOOK_CORE_SRCS "core/*.cpp")
add_library(OrderbookCore STATIC ${ORDERBOOK_CORE_SRCS})
file(GLOB ORDERBOOK_APP_SRCS "app/*.cpp")
add_library(OrderbookApp STATIC ${ORDERBOOK_APP_SRCS})
