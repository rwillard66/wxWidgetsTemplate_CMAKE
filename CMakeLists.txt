cmake_minimum_required(VERSION 2.8.9)

####################################################
# Project information
####################################################

PROJECT (mywxproject)
SET(TARGET_VERSION_MAJOR 0)
SET(TARGET_VERSION_MINOR 1)
SET(TARGET_VERSION_PATCH 0)
SET(VERSION "${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR}.${TARGET_VERSION_PATCH}")
SET(CPACK_PACKAGE_CONTACT "Rob Willard <robw@sagatech.ca>")
SET(TARGET_SHORT_DESCRIPTION "My application short description")
# TARGET_LONG_DESCRIPTION: each line must begin with a <space> and must not be
# longer than 80 characters.
SET(TARGET_LONG_DESCTIPTION " This is a template to create new wxWidgets based\n")
SET(TARGET_LONG_DESCTIPTION "${TARGET_LONG_DESCTIPTION} applications. Please view the README.txt file included in the\n")
SET(TARGET_LONG_DESCTIPTION "${TARGET_LONG_DESCTIPTION} root directory of this project.\n")

# Any additional source include directories
SET(ADDSRCDIR "")

# Add any Debian depends for packaging. This variable is comma separated
SET(DEPENDS "libwxbase3.0-0, libwxgtk3.0-0")
# Set Gnome icon location for .deb distribution
SET(CPACK_DEBIAN_PACKAGE_SECTION "Office")

####################################################
# Platform specific settings
####################################################
SET(RES_LOC "${CMAKE_CURRENT_LIST_DIR}/res")

SET(ICONTYPE ico)
IF(WIN32)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -mwindows -Wall")
ELSEIF(APPLE)
	SET(ICONTYPE icns)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
	INCLUDE_DIRECTORIES(/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include/c++/4.2.1)
ELSE() #LINUX
ENDIF()

####################################################
# IMAGE DEFINITION SECTION
####################################################
# Any images included in the application must be indicated here.
# Place the .png image in the res directory, and then add it to the appropriate
# size array below.
#
# 4 types of images:
#   SML   - for menus - 16x16
#   MED   - for toolbars and small buttons 24x24
#   LRG   - for large buttons 48x48
#   ICN   - for program icon

SET( IMAGES_SML
	)

SET( IMAGES_MED
	 )

SET( IMAGES_LRG
	)

# icon graphic must be saved as ${PROJECT_NAME}.png
SET( IMAGES_ICN ${PROJECT_NAME} )

####################################################
# Image creation
####################################################
SET( TMP_DIR "${CMAKE_BINARY_DIR}/CMakeFiles/CMakeTmp" )

FIND_PACKAGE(ImageMagick COMPONENTS convert REQUIRED)

FUNCTION (MAKE_XPM size imagelist)
	FOREACH(image ${imagelist})
		MESSAGE("  -- Converting ${image}.png to ${image}${size}.cpp")
		SET( PNG_FILE ${RES_LOC}/${image}.png )
		SET( XPM_FILE ${TMP_DIR}/${image}${size}.xpm )
		SET( CPP_FILE ${RES_LOC}/cpp/${image}${size}.cpp )
		EXECUTE_PROCESS( COMMAND ${ImageMagick_convert_EXECUTABLE} ${PNG_FILE} -resize ${size}x${size} ${XPM_FILE})
		EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy  ${XPM_FILE} ${CPP_FILE} )
		FILE(READ ${CPP_FILE} STATCPP)
		STRING(REPLACE "static" "const" CONSTCPP "${STATCPP}")
		FILE(WRITE ${CPP_FILE} "${CONSTCPP};")
	ENDFOREACH(image)
ENDFUNCTION(MAKE_XPM)

FUNCTION (MAKE_ICON image)
	MESSAGE("  -- Building icon: ${image}.${ICONTYPE}")
	EXECUTE_PROCESS(COMMAND ${ImageMagick_convert_EXECUTABLE} ${RES_LOC}/${image}.png ${RES_LOC}/${image}.${ICONTYPE})
ENDFUNCTION(MAKE_ICON)

MAKE_XPM(16 "${IMAGES_SML}")
MAKE_XPM(24 "${IMAGES_MED}")
MAKE_XPM(48 "${IMAGES_LRG}")
MAKE_ICON("${IMAGES_ICN}")

####################################################
# Build forms section
####################################################

EXECUTE_PROCESS( COMMAND wxformbuilder --generate --language=C++ ${RES_LOC}/forms.fbp)

####################################################
# Build Application section
####################################################

INCLUDE_DIRECTORIES(src res)
FILE(GLOB SOURCES "src/*.c*" "res/cpp/*.c*")

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})

IF(WIN32)
	SET(wxWidgets_CONFIGURATION msw)
	SET(wxWidgets_USE_STATIC yes)
ELSEIF(APPLE)
	SET(wxWidgets_ROOT_DIR "/wxWidgets-3.0.2/build-release/")
ENDIF(WIN32)

FIND_PACKAGE(wxWidgets COMPONENTS adv core base REQUIRED)
INCLUDE(${wxWidgets_USE_FILE})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

####################################################
# Packaging
####################################################

#common
SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}Installer")
SET(CPACK_PACKAGE_VERSION_MAJOR "${TARGET_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${TARGET_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${TARGET_VERSION_PATCH}")
SET(CPACK_PACKAGE_VERSION "${VERSION}")
SET(CPACK_PACKAGE_ICON "${RES_LOC}/${PROJECT_NAME}.${ICONTYPE}")
SET(CPACK_PACKAGE_VENDOR "SagaTech Electronics Inc.")
SET(CPACK_PACKAGE_HOMEPAGE "http://www.sagatech.ca")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${TARGET_SHORT_DESCRIPTION}")

IF(WIN32)
	#Create and link resource file for icons.
	SET(INFO_RC "id ICON \"${RES_LOC}/${PROJECT_NAME}.${ICONTYPE}\"\n")
	SET(INFO_RC "${INFO_RC}1 VERSIONINFO\n")
	SET(INFO_RC "${INFO_RC}FILEVERSION\t${TARGET_VERSION_MAJOR},${TARGET_VERSION_MINOR},${TARGET_VERSION_PATCH}\n")
	SET(INFO_RC "${INFO_RC}PRODUCTVERSION\t${TARGET_VERSION_MAJOR},${TARGET_VERSION_MINOR},${TARGET_VERSION_PATCH}\n")
	SET(INFO_RC "${INFO_RC}BEGIN\n")
	SET(INFO_RC "${INFO_RC} BLOCK \"StringFileInfo\"\n")
	SET(INFO_RC "${INFO_RC} BEGIN\n")
	SET(INFO_RC "${INFO_RC}  BLOCK \"080904E4\"\n")
	SET(INFO_RC "${INFO_RC}  BEGIN\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"CompanyName\", \"SagaTech Electronics, Inc.\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"FileDescription\", \"${TARGET_SHORT_DESCRIPTION}\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"FileVersion\", \"${VERSION}\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"InternalName\", \"${PROJECT_NAME}\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"LegalCopyright\", \"(c) 2015 SagaTech Electronics, Inc.\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"OriginalFilename\", \"${PROJECT_NAME}\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"ProductName\", \"${PROJECT_NAME}\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"ProductVersion\", \"${VERSION}\"\n")
	SET(INFO_RC "${INFO_RC}  END\n")
	SET(INFO_RC "${INFO_RC} END\n\n")
	SET(INFO_RC "${INFO_RC} BLOCK \"VarFileInfo\"\n")
	SET(INFO_RC "${INFO_RC} BEGIN\n")
	SET(INFO_RC "${INFO_RC}  VALUE \"Translation\", 0x809, 1252\n")
	SET(INFO_RC "${INFO_RC} END\n")
	SET(INFO_RC "${INFO_RC}END\n")
	FILE(WRITE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.rc" "${INFO_RC}")
	EXECUTE_PROCESS(COMMAND windres "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.rc" -O coff -o "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.res")
	#link resources into executable
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.res)
	#set cpack variables
	SET(CPACK_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
	INCLUDE(InstallRequiredSystemLibraries)
	INSTALL(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
        DESTINATION programs
        COMPONENT applications)
	INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
	SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Sagatech\\\\${PROJECT_NAME}")
	SET(CPACK_PACKAGE_ICON "${RES_LOC}\\\\${PROJECT_NAME}.png")
	SET(CPACK_NSIS_MUI_ICON "${RES_LOC}\\\\${PROJECT_NAME}.${ICONTYPE}")
	SET(CPACK_NSIS_MUI_UNIICON "${RES_LOC}\\\\${PROJECT_NAME}.${ICONTYPE}")
	SET(CPACK_NSIS_MENU_LINKS "${PROJECT_NAME}" "${PROJECT_NAME}")
ELSEIF(APPLE)
	# Create Info.plist file for bundle
	SET(INFO_FILE "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
	SET(INFO_FILE "${INFO_FILE}<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n")
	SET(INFO_FILE "${INFO_FILE}<plist version=\"1.0\">\n")
	SET(INFO_FILE "${INFO_FILE}<dict>\n")
	SET(INFO_FILE "${INFO_FILE}   <key>CFBundleDevelopmentRegion</key>\n")
	SET(INFO_FILE "${INFO_FILE}   <string>English</string>\n")
	SET(INFO_FILE "${INFO_FILE}   <key>CFBundleExecutable</key>\n")
	SET(INFO_FILE "${INFO_FILE}   <string>${PROJECT_NAME}</string>\n")
	SET(INFO_FILE "${INFO_FILE}   <key>CFBundleIconFile</key>\n")
	SET(INFO_FILE "${INFO_FILE}   <string>${PROJECT_NAME}.${ICONTYPE}</string>\n")
	SET(INFO_FILE "${INFO_FILE}   <key>CFBundleName</key>\n")
	SET(INFO_FILE "${INFO_FILE}   <string>${PROJECT_NAME}</string>\n")
	SET(INFO_FILE "${INFO_FILE}   <key>CFBundleIdentifier</key>\n")
	SET(INFO_FILE "${INFO_FILE}   <string>com.sagatech.${PROJECT_NAME}</string>\n")
	SET(INFO_FILE "${INFO_FILE}   <key>CFBundleInfoDictionaryVersion</key>\n")
	SET(INFO_FILE "${INFO_FILE}   <string>6.0</string>\n")
	SET(INFO_FILE "${INFO_FILE}   <key>CFBundlePackageType</key>\n")
	SET(INFO_FILE "${INFO_FILE}   <string>APPL</string>\n")
	SET(INFO_FILE "${INFO_FILE}   <key>CFBundleVersion</key>\n")
	SET(INFO_FILE "${INFO_FILE}   <string>${VERSION}</string>\n")
	SET(INFO_FILE "${INFO_FILE}   <key>CFBundleShortVersionString</key>\n")
	SET(INFO_FILE "${INFO_FILE}   <string>${VERSION}</string>\n")
	SET(INFO_FILE "${INFO_FILE}   <key>CFBundleGetInfoString</key>\n")
	SET(INFO_FILE "${INFO_FILE}   <string>${PROJECT_NAME}, Version ${VERSION}, SagaTech Electronics, Inc.</string>\n")
	SET(INFO_FILE "${INFO_FILE}</dict>\n")
	SET(INFO_FILE "${INFO_FILE}</plist>\n")
	FILE(WRITE "${CMAKE_BINARY_DIR}/Info.plist" "${INFO_FILE}")
	# Set CPACK Variables and install locations
	SET(CPACK_GENERATOR "Bundle")
	SET(CPACK_BUNDLE_NAME "${PROJECT_NAME}")
	SET(CPACK_BUNDLE_PLIST "${CMAKE_BINARY_DIR}/Info.plist")
	SET(CPACK_BUNDLE_ICON "${RES_LOC}/${PROJECT_NAME}.${ICONTYPE}")
	INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ../MacOS)
ELSE() #LINUX
	# Create .desktop file for application launcher
	SET(DESKTOP_FILE "[Desktop Entry]\nName[en_CA]=${PROJECT_NAME}\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Comment=${TARGET_SHORT_DESCRIPTION}\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Icon=/usr/share/${PROJECT_NAME}/${PROJECT_NAME}.${ICONTYPE}\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Exec=/usr/bin/${PROJECT_NAME}\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Version=${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR}.${TARGET_VERSION_PACKAGE}\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Type=Application\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Terminal=false\n")
	FILE(WRITE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop" "${DESKTOP_FILE}")
	# Set CPACK Variables and install locations
	SET(CPACK_GENERATOR "DEB")
	SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
	SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${TARGET_SHORT_DESCRIPTION}\n ${TARGET_LONG_DESCTIPTION}")
	SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.sagatech.ca")
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${DEPENDS}")
	INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop DESTINATION /usr/share/applications)
	INSTALL(FILES ${RES_LOC}/${PROJECT_NAME}.${ICONTYPE} DESTINATION /usr/share/${PROJECT_NAME})
ENDIF()
INCLUDE(CPack)
