project "GeconPC"
    kind "WindowedApp"
    language "C++"

    targetdir "../bin"

    uses "QtGui"
    uses "QtTest"

    require_pkg_config()
    require_pkg("GeconFramework")
    require_pkg("FakeInput")
    require_pkg("x11")

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
        "ObjectPolicy.*",
        "ObjectModel.*",
        "ObjectWrapper.*",
        "GestureModel.*",
        "GestureWrapper.*",
        "ObjectStateSettings.*",
        "PropertyRelationSettings.*",
        "ObjectRelationSettings.*",
        "PropertyValueSettings.*",
        "TestDialog.*",
        "ActionTriggerDialog.*",
        "GestureEventDialog.*",
        "EventWrapper.*",
        "ActionSettings.*",
        "ActionTriggerModel.*",
        "ActionTriggerWrapper.*",
        "DebugDialog.*",
        "DialogBase.*"
    }
