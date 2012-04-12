project "GeconPC"
    kind "WindowedApp"
    language "C++"

    targetdir "../bin"

    uses "QtGui"

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
        "SettingsDialog.*",
        "Capture.*",
        "ObjectModel.*",
        "ObjectWrapper.*",
        "GestureModel.*",
        "GestureWrapper.*",
        "StateGestureWrapper.*",
        "ObjectPropertyStateSettings.*"
    }
