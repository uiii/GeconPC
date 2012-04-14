project "GeconPC"
    kind "WindowedApp"
    language "C++"

    targetdir "../bin"

    uses "QtGui"
    uses "QtTest"

    require_pkg_config()
    require_pkg("GeconFramework")

    includedirs { "./" }

    files {
        "main.cpp",
        "MainWindow.*",
        "ImageDisplay.*",
        "ObjectDialog.*",
        "StateGestureDialog.*",
        "RelationGestureDialog.*",
        "MotionGestureDialog.*",
        "MotionGestureWrapper.*",
        "MotionRecorder.*",
        "SettingsDialog.*",
        "Capture.*",
        "ObjectPolicy.*",
        "ObjectModel.*",
        "ObjectWrapper.*",
        "GestureModel.*",
        "GestureWrapper.*",
        "StateGestureWrapper.*",
        "ObjectPropertyStateSettings.*"
    }
