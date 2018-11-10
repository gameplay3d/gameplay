QMAKE_CLEAN += $$DESTDIR/$$TARGET

TEMPLATE = subdirs

CONFIG = ordered

SUBDIRS += \
    gameplay/gameplay.pro \
    gameplay-app/gameplay-app.pro \
    gameplay-editor/gameplay-editor.pro \

