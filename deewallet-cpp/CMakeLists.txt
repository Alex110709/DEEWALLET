cmake_minimum_required(VERSION 3.20)
project(DEEWallet VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find vcpkg toolchain
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Project sources
set(SOURCES
    src/main.cpp
    src/core/WalletCore.cpp
    src/core/KeyfileManager.cpp
    src/core/SecureMemory.cpp
    src/core/BIP39.cpp
    src/core/BIP32.cpp
    src/utils/AddressUtils.cpp
    src/utils/Keccak256.cpp
    src/utils/TransactionBuilder.cpp
    src/utils/TokenDetector.cpp
    src/chains/BitcoinAdapter.cpp
    src/chains/EthereumAdapter.cpp
    src/chains/TronAdapter.cpp
    src/chains/SolanaAdapter.cpp
    src/rpc/RPCManager.cpp
    src/rpc/RPCClient.cpp
    src/ui/MainWindow.cpp
    src/ui/WelcomeScreen.cpp
    src/ui/CreateWalletDialog.cpp
    src/ui/ImportWalletDialog.cpp
    src/ui/WalletDetailScreen.cpp
    src/ui/SendTransactionDialog.cpp
    src/ui/QRCodeDialog.cpp
    src/ui/AddressBookDialog.cpp
)

set(HEADERS
    src/core/WalletCore.h
    src/core/KeyfileManager.h
    src/core/SecureMemory.h
    src/core/BIP39.h
    src/core/BIP32.h
    src/utils/AddressUtils.h
    src/utils/Keccak256.h
    src/utils/TransactionBuilder.h
    src/utils/TokenDetector.h
    src/chains/ChainAdapter.h
    src/chains/BitcoinAdapter.h
    src/chains/EthereumAdapter.h
    src/chains/TronAdapter.h
    src/chains/SolanaAdapter.h
    src/rpc/RPCManager.h
    src/rpc/RPCClient.h
    src/ui/MainWindow.h
    src/ui/WelcomeScreen.h
    src/ui/CreateWalletDialog.h
    src/ui/ImportWalletDialog.h
    src/ui/WalletDetailScreen.h
    src/ui/SendTransactionDialog.h
    src/ui/QRCodeDialog.h
    src/ui/AddressBookDialog.h
)

# Create executable
add_executable(deewallet ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(deewallet PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Include directories
target_include_directories(deewallet PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OPENSSL_INCLUDE_DIR}
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(deewallet PRIVATE WIN32_LEAN_AND_MEAN)
    set_target_properties(deewallet PROPERTIES WIN32_EXECUTABLE TRUE)
elseif(APPLE)
    set_target_properties(deewallet PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist
    )
endif()

# Install rules
install(TARGETS deewallet
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# Copy Qt plugins (for static deployment)
if(WIN32 OR APPLE)
    install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/deewallet\" \"\" \"\")
    ")
endif()
