QT = core gui widgets
TARGET = zedit
TEMPLATE = app
INCLUDEPATH += ./src

QMAKE_CXXFLAGS = -std=c++11
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

MOC_DIR = .moc
OBJECTS_DIR = .obj

SOURCES += src/main.cpp \
    src/gui/mainwindow.cpp \
    src/types/misc/zfile_io.cpp \
    src/types/misc/pce_types.cpp \
    src/common/global.cpp \
    src/types/widgets/color_select.cpp \
    src/gui/tabs/map_editor_tab.cpp \
    src/gui/tabs/palette_editor_tab.cpp \
    src/gui/tabs/tab_base.cpp \
    src/types/widgets/color_display_widget.cpp \
    src/types/scenes/block_graphics_scene.cpp \
    src/types/views/block_graphics_view.cpp \
    src/gui/tabs/block_editor_tab.cpp \
    src/types/widgets/specialized/tools.cpp \
    src/types/scenes/block_editor_scene.cpp \
    src/types/items/graphics_image_item.cpp \
    src/types/widgets/specialized/palette_buttons.cpp \
    src/types/widgets/colored_push_button.cpp \
    src/types/undo_commands/undo_data_alteration.cpp \
    src/types/items/graphics_outlined_polygon.cpp \
    src/types/undo_commands/undo_select_blocks.cpp \
    src/types/undo_commands/undo_image_item_alteration.cpp \
    src/types/widgets/specialized/editing_toolbar.cpp \
    src/types/widgets/one_digit_spinbox.cpp \
    src/types/misc/linked_image.cpp \
    src/types/undo_commands/undo_stack.cpp \
    src/types/widgets/multibutton.cpp \
    src/types/items/block_button_item.cpp

HEADERS += src/gui/mainwindow.h \
    src/types/misc/pce_types.h \
    src/types/misc/template_array.h \
    src/types/misc/zfile_io.h \
    src/common/global.h \
    src/common/color_converters.h \
    src/types/widgets/color_select.h \
    src/gui/tabs/map_editor_tab.h \
    src/gui/tabs/palette_editor_tab.h \
    src/gui/tabs/tab_base.h \
    src/types/widgets/color_display_widget.h \
    src/types/scenes/block_graphics_scene.h \
    src/types/views/block_graphics_view.h \
    src/types/widgets/custom_action.h \
    src/types/misc/anonymous_struct.h \
    src/gui/tabs/block_editor_tab.h \
    src/types/widgets/specialized/tools.h \
    src/types/scenes/block_editor_scene.h \
    src/types/items/graphics_image_item.h \
    src/types/misc/mmvector.h \
    src/types/widgets/specialized/palette_buttons.h \
    src/types/widgets/colored_push_button.h \
    src/types/undo_commands/undo_data_alteration.h \
    src/types/items/graphics_outlined_polygon.h \
    src/types/undo_commands/undo_select_blocks.h \
    src/types/undo_commands/undo_image_item_alteration.h \
    src/types/widgets/specialized/editing_toolbar.h \
    src/common/labmda_slot.h \
    src/types/widgets/one_digit_spinbox.h \
    src/types/misc/linked_image.h \
    src/types/undo_commands/undo_stack.h \
    src/common/nullable.h \
    src/types/widgets/multibutton.h \
    src/types/items/block_button_item.h \
    src/types/misc/matrix.h \
    src/types/misc/structs.h

RESOURCES += src/resources/map_tools.qrc \
    src/resources/other.qrc
unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

OTHER_FILES = \
    src/types/misc/template_array.cpp \
    src/types/misc/mmvector.cpp
