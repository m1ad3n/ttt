
set(CORE_FILES "ttt.c" "ttt.h")

target_sources(ttt_cli PRIVATE ttt_cli.c ${CORE_FILES})
target_sources(ttt_gui PRIVATE ttt_gui.c ${CORE_FILES})