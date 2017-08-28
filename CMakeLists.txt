#	(c) 2016 Rob Willard <rwillard66@gmail.com>
#
#    This file is part of wxWidgetsTemplate.
#
#    wxWidgetsTemplate is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    wxWidgetsTemplate is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with wxWidgetsTemplate.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8.9)

####################################################
# Project information
####################################################

PROJECT (mywxproject)
SET(TARGET_VERSION_MAJOR 0)
SET(TARGET_VERSION_MINOR 1)
SET(TARGET_VERSION_PATCH 0)
#set target version
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERMAJ=${TARGET_VERSION_MAJOR} -DVERMIN=${TARGET_VERSION_MINOR} -DVERPAT=${TARGET_VERSION_PATCH}")
SET(VERSION "${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR}.${TARGET_VERSION_PATCH}")
SET(CPACK_PACKAGE_CONTACT "Rob Willard <rwillard66@gmail.com>")
SET(TARGET_SHORT_DESCRIPTION "My application short description")
# TARGET_LONG_DESCRIPTION: each line must begin with a <space> and must not be
# longer than 80 characters.
SET(TARGET_LONG_DESCTIPTION " This is a template to create new wxWidgets based\n")
SET(TARGET_LONG_DESCTIPTION "${TARGET_LONG_DESCTIPTION} applications. Please view the README.txt file included in the\n")
SET(TARGET_LONG_DESCTIPTION "${TARGET_LONG_DESCTIPTION} root directory of this project.\n")

OPTION(DOALL "DOALL" OFF)
OPTION(USE_FB "USE_FB" OFF) # To use formbuilding, set to ONs

#disable CMAKE developer warnings to supress warning under windows
IF(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
     SET(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
ENDIF(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)

# Any additional source include directories
SET(ADDSRCDIR "")

# autogenerate dependency information
set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# Set Gnome icon location for .deb distribution
SET(CPACK_DEBIAN_PACKAGE_SECTION "Office")

#specify location of custom cmake modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

#set temp directory for auto generated files
SET( TMP_DIR "${CMAKE_BINARY_DIR}/CMakeFiles/CMakeTmp" )

####################################################
# INCLUDE DIRECTORIES AND FILES
####################################################

INCLUDE_DIRECTORIES(src res "../common/")
FILE(GLOB SOURCES "src/*.c*" "${TMP_DIR}/cpp/*.c*" "../common/*.c*")

####################################################
# IMAGE DEFINITION SECTION
####################################################
# Any images included in the application must be indicated here.
# Place the image in the res directory, and then add it to the appropriate
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
	connect.svg
	disconnect.svg
	 )

SET( IMAGES_LRG
	${PROJECT_NAME}.svg
	)

SET( IMAGES_CUST
	banner.svg
	)

# icon graphic must be saved as ${PROJECT_NAME}.svg
SET( IMAGES_ICN ${PROJECT_NAME}.svg )

####################################################
# Platform specific settings
####################################################
SET(RES_LOC "${CMAKE_CURRENT_LIST_DIR}/res")

ADD_DEFINITIONS(-DTIXML_USE_STL)

SET(ICONTYPE ico)
IF(WIN32)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -mwindows")
ELSEIF(APPLE)
	SET(ICONTYPE icns)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
	INCLUDE_DIRECTORIES(/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include/c++/4.2.1)
ELSE() #LINUX
#	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF()

####################################################
# First run check
####################################################

IF(NOT EXISTS ${TMP_DIR}/${PROJECT_NAME}.${ICONTYPE})
	# if the project icon does not exist, we assume this is a first run and
	# override defaults and command line switches to generate everything
	SET(DOALL ON)
ENDIF(NOT EXISTS  ${TMP_DIR}/${PROJECT_NAME}.${ICONTYPE})

IF(NOT DOALL)
	MESSAGE("==================================================================")
	MESSAGE("Quick Building ${PROJECT_NAME}\n")
	MESSAGE("  To rebuild forms, help and images, run cmake")
	MESSAGE("  again with the -DDOALL=ON option")
	MESSAGE("==================================================================")
ENDIF(NOT DOALL)

####################################################
# Library discovery.
####################################################
MESSAGE("== Discovering Libraries ==")

IF(DOALL)
	# Image magick for graphics conversion
	FIND_PACKAGE(ImageMagick COMPONENTS convert REQUIRED)
ENDIF(DOALL)

#wxWidgets
IF(WIN32)
	SET(wxWidgets_CONFIGURATION msw)
	SET(wxWidgets_USE_STATIC yes)
ELSEIF(APPLE)
	SET(wxWidgets_ROOT_DIR "/wxWidgets-3.0.2/build-release/")
ENDIF(WIN32)
FIND_PACKAGE(wxWidgets COMPONENTS adv xrc gl html core base REQUIRED)
INCLUDE(${wxWidgets_USE_FILE})
INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS})

# Add any additional libraries to the following 
SET(MYLIBS ${wxWidgets_LIBRARIES})

MESSAGE("== Library discovery complete ==")

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
# Image creation
####################################################
FUNCTION (MAKE_XPM sizex sizey imagelist)
	FOREACH(image ${imagelist})
		GET_FILENAME_COMPONENT(IMG_NAME ${image} NAME_WE)
		MESSAGE("  -- Converting ${image} to ${IMG_NAME}${sizex}.cpp")
		SET( IN_FILE ${RES_LOC}/${image} )
		EXECUTE_PROCESS( COMMAND ${ImageMagick_convert_EXECUTABLE} -background none ${IN_FILE} -resize  ${sizex}x${sizey} ${TMP_DIR}/${IMG_NAME}${sizex}.xpm)
		EXECUTE_PROCESS( COMMAND ${ImageMagick_convert_EXECUTABLE} -background none ${IN_FILE} -resize ${sizex}x${sizey} -type Grayscale ${TMP_DIR}/${IMG_NAME}_disabled${sizex}.xpm)
		EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy ${TMP_DIR}/${IMG_NAME}${sizex}.xpm  ${TMP_DIR}/cpp/${IMG_NAME}${sizex}.cpp )
		EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy ${TMP_DIR}/${IMG_NAME}_disabled${sizex}.xpm  ${TMP_DIR}/cpp/${IMG_NAME}_disabled${sizex}.cpp )
		FILE(READ ${TMP_DIR}/cpp/${IMG_NAME}${sizex}.cpp STATCPP)
		STRING(REPLACE "static" "const" CONSTCPP "${STATCPP}")
		FILE(WRITE ${TMP_DIR}/cpp/${IMG_NAME}${sizex}.cpp "${CONSTCPP};")
		FILE(READ ${TMP_DIR}/cpp/${IMG_NAME}_disabled${sizex}.cpp STATCPP)
		STRING(REPLACE "static" "const" CONSTCPP "${STATCPP}")
		FILE(WRITE ${TMP_DIR}/cpp/${IMG_NAME}_disabled${sizex}.cpp "${CONSTCPP};")
	ENDFOREACH(image)
ENDFUNCTION(MAKE_XPM)

FUNCTION (MAKE_ICON image)
	MESSAGE("  -- Building icon: ${PROJECT_NAME}.${ICONTYPE}")
	EXECUTE_PROCESS(COMMAND ${ImageMagick_convert_EXECUTABLE} ${RES_LOC}/${image} -resize 48x48 ${TMP_DIR}/${PROJECT_NAME}.${ICONTYPE})
ENDFUNCTION(MAKE_ICON)

IF(DOALL)
	MESSAGE("== Building Graphics ==")
	MAKE_XPM(16 16 "${IMAGES_SML}")
	MAKE_XPM(24 24 "${IMAGES_MED}")
	MAKE_XPM(48 48 "${IMAGES_LRG}")
	MAKE_XPM(100 350 "${IMAGES_CUST}")
	MAKE_ICON("${IMAGES_ICN}")
	MESSAGE("== Building Graphics Complete ==")
	IF(USE_FB)
		####################################################
		# Build forms section
		####################################################
		MESSAGE("== Building Forms ==")

		EXECUTE_PROCESS( COMMAND wxformbuilder --generate --language=XRC ${RES_LOC}/forms.fbp)
	ENDIF(USE_FB)
	####################################################
	# Set default for future CMAkes to DOALL = OFF
	####################################################
	SET(DOALL OFF)

ENDIF(DOALL)

####################################################
# Build Application section
####################################################

MESSAGE("== Linking ==")

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${MYLIBS})

####################################################
# Packaging
####################################################
MESSAGE("== Packaging ==")
#common
SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}Installer")
SET(CPACK_PACKAGE_VERSION_MAJOR "${TARGET_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${TARGET_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${TARGET_VERSION_PATCH}")
SET(CPACK_PACKAGE_VERSION "${VERSION}")
SET(CPACK_PACKAGE_ICON "${TMP_DIR}/${PROJECT_NAME}.${ICONTYPE}")
SET(CPACK_PACKAGE_VENDOR "Rob Willard")
SET(CPACK_PACKAGE_HOMEPAGE "http://")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${TARGET_SHORT_DESCRIPTION}")

IF(WIN32)
	#Create and link resource file for icons.
	SET(INFO_RC "id ICON \"${TMP_DIR}/${PROJECT_NAME}.${ICONTYPE}\"\n")
	SET(INFO_RC "${INFO_RC}1 VERSIONINFO\n")
	SET(INFO_RC "${INFO_RC}FILEVERSION\t${TARGET_VERSION_MAJOR},${TARGET_VERSION_MINOR},${TARGET_VERSION_PATCH}\n")
	SET(INFO_RC "${INFO_RC}PRODUCTVERSION\t${TARGET_VERSION_MAJOR},${TARGET_VERSION_MINOR},${TARGET_VERSION_PATCH}\n")
	SET(INFO_RC "${INFO_RC}BEGIN\n")
	SET(INFO_RC "${INFO_RC} BLOCK \"StringFileInfo\"\n")
	SET(INFO_RC "${INFO_RC} BEGIN\n")
	SET(INFO_RC "${INFO_RC}  BLOCK \"080904E4\"\n")
	SET(INFO_RC "${INFO_RC}  BEGIN\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"CompanyName\", \"Rob Willard.\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"FileDescription\", \"${TARGET_SHORT_DESCRIPTION}\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"FileVersion\", \"${VERSION}\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"InternalName\", \"${PROJECT_NAME}\"\n")
	SET(INFO_RC "${INFO_RC}    VALUE \"LegalCopyright\", \"(c) 2017 Rob Willard\"\n")
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
	SET(CPACK_PACKAGE_ICON "${TMP_DIR}\\\\${PROJECT_NAME}.${ICONTYPE}")
	SET(CPACK_NSIS_MUI_ICON "${TMP_DIR}\\\\${PROJECT_NAME}.${ICONTYPE}")
	SET(CPACK_NSIS_MUI_UNIICON "${TMP_DIR}\\\\${PROJECT_NAME}.${ICONTYPE}")
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
	SET(INFO_FILE "${INFO_FILE}   <string>${PROJECT_NAME}, Version ${VERSION}, Rob Willard.</string>\n")
	SET(INFO_FILE "${INFO_FILE}</dict>\n")
	SET(INFO_FILE "${INFO_FILE}</plist>\n")
	FILE(WRITE "${CMAKE_BINARY_DIR}/Info.plist" "${INFO_FILE}")
	# Set CPACK Variables and install locations
	SET(CPACK_GENERATOR "Bundle")
	SET(CPACK_BUNDLE_NAME "${PROJECT_NAME}")
	SET(CPACK_BUNDLE_PLIST "${CMAKE_BINARY_DIR}/Info.plist")
	SET(CPACK_BUNDLE_ICON "${TMP_DIR}/${PROJECT_NAME}.${ICONTYPE}")
	INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ../MacOS)
ELSE() #LINUX
	# Create .desktop file for application launcher
	SET(DESKTOP_FILE "[Desktop Entry]\nName[en_CA]=${PROJECT_NAME}\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Comment=${TARGET_SHORT_DESCRIPTION}\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Icon=/usr/share/${PROJECT_NAME}/${PROJECT_NAME}.${ICONTYPE}\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Exec=/usr/bin/${PROJECT_NAME}\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Version=${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR}.${TARGET_VERSION_PATCH}\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Type=Application\n")
	SET(DESKTOP_FILE "${DESKTOP_FILE}Terminal=false\n")
	FILE(WRITE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop" "${DESKTOP_FILE}")
	# Set CPACK Variables and install locations
	SET(CPACK_GENERATOR "DEB")
	SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
	SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${TARGET_SHORT_DESCRIPTION}\n ${TARGET_LONG_DESCTIPTION}")
	SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://")
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${DEPENDS}")
	INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
	INSTALL(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop DESTINATION /usr/share/applications)
	INSTALL(FILES ${TMP_DIR}/${PROJECT_NAME}.${ICONTYPE} DESTINATION /usr/share/${PROJECT_NAME})
ENDIF()
INCLUDE(CPack)
