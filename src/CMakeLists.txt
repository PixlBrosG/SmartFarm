# Common library (protocol, serialization, etc.)
add_library(SmartFarm_Common
        Common/Protocol.cpp
        Common/Message.cpp
        Common/Logger.cpp
)
target_include_directories(SmartFarm_Common PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
# Link asio + threads
target_link_libraries(SmartFarm_Common
    PUBLIC
        Threads::Threads
)
if (Asio_FOUND)
    target_link_libraries(SmartFarm_Common PUBLIC Asio::asio)
else()
    target_include_directories(SmartFarm_Common PUBLIC ${asio_SOURCE_DIR}/asio/include)
endif()

# ---- Sensor node ----
add_executable(SensorNode
        Sensor/Main.cpp
        Sensor/SensorNode.cpp
)
target_link_libraries(SensorNode PRIVATE SmartFarm_Common)

# ---- Control panel ----
add_executable(ControlPanel
        Control/Main.cpp
        Control/ControlPanel.cpp
)
target_link_libraries(ControlPanel PRIVATE SmartFarm_Common)

# ---- Server ----
add_executable(Server
        Server/Main.cpp
        Server/Server.cpp
)
target_link_libraries(Server PRIVATE SmartFarm_Common)
