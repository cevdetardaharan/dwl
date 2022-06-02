include config.mk
CFLAGS += -I. -DWLR_USE_UNSTABLE -std=c99 -pedantic -DVERSION=\"$(VERSION)\"
WAYLAND_PROTOCOLS=$(shell pkg-config --variable=pkgdatadir wayland-protocols)
WAYLAND_SCANNER=$(shell pkg-config --variable=wayland_scanner wayland-scanner)
PKGS = wlroots wayland-server xcb xkbcommon libinput
CFLAGS += $(foreach p,$(PKGS),$(shell pkg-config --cflags $(p)))
LDLIBS += $(foreach p,$(PKGS),$(shell pkg-config --libs $(p)))
clean:
	rm -f dwl *.o *-protocol.h *-protocol.c
install: dwl
	install -Dm755 dwl $(DESTDIR)$(PREFIX)/bin/dwl
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/dwl
xdg-shell-protocol.h:
	$(WAYLAND_SCANNER) server-header \
		$(WAYLAND_PROTOCOLS)/stable/xdg-shell/xdg-shell.xml $@
xdg-shell-protocol.c:
	$(WAYLAND_SCANNER) private-code \
		$(WAYLAND_PROTOCOLS)/stable/xdg-shell/xdg-shell.xml $@
xdg-shell-protocol.o: xdg-shell-protocol.h
wlr-layer-shell-unstable-v1-protocol.h:
	$(WAYLAND_SCANNER) server-header \
		protocols/wlr-layer-shell-unstable-v1.xml $@
wlr-layer-shell-unstable-v1-protocol.c:
	$(WAYLAND_SCANNER) private-code \
		protocols/wlr-layer-shell-unstable-v1.xml $@
wlr-layer-shell-unstable-v1-protocol.o: wlr-layer-shell-unstable-v1-protocol.h
idle-protocol.h:
	$(WAYLAND_SCANNER) server-header \
		protocols/idle.xml $@
idle-protocol.c:
	$(WAYLAND_SCANNER) private-code \
		protocols/idle.xml $@
idle-protocol.o: idle-protocol.h
dwl.o: config.mk config.h client.h xdg-shell-protocol.h wlr-layer-shell-unstable-v1-protocol.h idle-protocol.h util.h
dwl: xdg-shell-protocol.o wlr-layer-shell-unstable-v1-protocol.o idle-protocol.o util.o
