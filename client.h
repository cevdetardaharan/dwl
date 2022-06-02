static inline struct wlr_surface *
client_surface(Client *c)
{
	return c->surface.xdg->surface;
}
static inline void
client_activate_surface(struct wlr_surface *s, int activated)
{
	struct wlr_xdg_surface *surface;
	if (wlr_surface_is_xdg_surface(s)
			&& (surface = wlr_xdg_surface_from_wlr_surface(s))
			&& surface->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL)
		wlr_xdg_toplevel_set_activated(surface, activated);
}
static inline void
client_for_each_surface(Client *c, wlr_surface_iterator_func_t fn, void *data)
{
	wlr_surface_for_each_surface(client_surface(c), fn, data);
	wlr_xdg_surface_for_each_popup_surface(c->surface.xdg, fn, data);
}
static inline const char *
client_get_appid(Client *c)
{
	return c->surface.xdg->toplevel->app_id;
}
static inline void
client_get_geometry(Client *c, struct wlr_box *geom)
{
	wlr_xdg_surface_get_geometry(c->surface.xdg, geom);
}

static inline const char *
client_get_title(Client *c)
{
	return c->surface.xdg->toplevel->title;
}
static inline int
client_is_float_type(Client *c)
{
	struct wlr_xdg_toplevel *toplevel;
	struct wlr_xdg_toplevel_state state;
	toplevel = c->surface.xdg->toplevel;
	state = toplevel->current;
	return (state.min_width != 0 && state.min_height != 0
		&& (state.min_width == state.max_width
		|| state.min_height == state.max_height))
		|| toplevel->parent;
}
static inline int
client_wants_fullscreen(Client *c)
{
	return c->surface.xdg->toplevel->requested.fullscreen;
}
static inline int
client_is_unmanaged(Client *c)
{
	return 0;
}
static inline void
client_send_close(Client *c)
{
	wlr_xdg_toplevel_send_close(c->surface.xdg);
}
static inline void
client_set_fullscreen(Client *c, int fullscreen)
{
	wlr_xdg_toplevel_set_fullscreen(c->surface.xdg, fullscreen);
}
static inline uint32_t
client_set_size(Client *c, uint32_t width, uint32_t height)
{
	return wlr_xdg_toplevel_set_size(c->surface.xdg, width, height);
}
static inline void
client_set_tiled(Client *c, uint32_t edges)
{
	wlr_xdg_toplevel_set_tiled(c->surface.xdg, edges);
}
static inline struct wlr_surface *
client_surface_at(Client *c, double cx, double cy, double *sx, double *sy)
{
	return wlr_xdg_surface_surface_at(c->surface.xdg, cx, cy, sx, sy);
}
static inline void
client_min_size(Client *c, int *width, int *height)
{
	struct wlr_xdg_toplevel *toplevel;
	struct wlr_xdg_toplevel_state *state;
	toplevel = c->surface.xdg->toplevel;
	state = &toplevel->current;
	*width = state->min_width;
	*height = state->min_height;
}
static inline Client *
client_from_wlr_surface(struct wlr_surface *surface)
{
	struct wlr_scene_node *n = surface->data;
	return n ? n->data : NULL;
}
static inline Client *
client_from_popup(struct wlr_xdg_popup *popup)
{
	struct wlr_xdg_surface *surface = popup->base;
	while (1) {
		switch (surface->role) {
		case WLR_XDG_SURFACE_ROLE_POPUP:
			if (!wlr_surface_is_xdg_surface(surface->popup->parent))
				return NULL;
			surface = wlr_xdg_surface_from_wlr_surface(surface->popup->parent);
			break;
		case WLR_XDG_SURFACE_ROLE_TOPLEVEL:
				return surface->data;
		case WLR_XDG_SURFACE_ROLE_NONE:
			return NULL;
		}
	}
}
