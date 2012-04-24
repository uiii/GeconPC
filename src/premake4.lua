project "GeconPC"
    kind "WindowedApp"
    language "C++"

    targetdir "../bin"

    uses "QtGui"
    uses "QtTest"

    require_pkg_config()
    require_pkg("GeconFramework")
    require_pkg("FakeInput")

    includedirs { "./" }

    files {
        "main.cpp",
        "MainWindow.*",
        "ImageDisplay.*",
        "ObjectDialog.*",
        "StateGestureDialog.*",
        "RelationGestureDialog.*",
        "MotionGestureDialog.*",
        "StateGestureWrapper.*",
        "RelationGestureWrapper.*",
        "MotionGestureWrapper.*",
        "MotionRecorder.*",
        "SettingsDialog.*",
        "Capture.*",
        "ObjectPolicy.*",
        "ObjectModel.*",
        "ObjectWrapper.*",
        "GestureModel.*",
        "GestureWrapper.*",
        "ObjectStateSettings.*",
        "PropertyRelationSettings.*",
        "ObjectRelationSettings.*",
        "PropertyValueSettings.*",
        "GestureTestDialog.*",
        "EventTriggerDialog.*",
        "GestureEventDialog.*",
        "EventWrapper.*",
        "ActionSettings.*",
        "EventTriggerModel.*",
        "EventTriggerWrapper.*",
        "DebugDialog.*"
    }
