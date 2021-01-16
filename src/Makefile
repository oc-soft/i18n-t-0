
topbuild_dir=../bin/
LINK=$(CC)
MSGFMT = msgfmt
MKDIR_P = mkdir -p
CFLAGS= -g
RM=rm -f
RM_R=rm -f -r

i18n_msg_OBJECTS = $(topbuild_dir)main.o 

i18n_msg_MOFILES = $(topbuild_dir)i18n/ja/LC_MESSAGES/messages.mo \
	$(topbuild_dir)i18n/ja_JP/LC_MESSAGES/messages.mo \
	$(topbuild_dir)i18n/en/LC_MESSAGES/messages.mo \
	$(topbuild_dir)i18n/en_US/LC_MESSAGES/messages.mo


all: $(topbuild_dir)i18n-msg $(i18n_msg_MOFILES)

$(topbuild_dir)i18n-msg: $(i18n_msg_OBJECTS)
	$(LINK) -o $@ $^ $(LDFLAGS)


$(topbuild_dir)main.o: main.c

$(topbuild_dir)i18n/ja/LC_MESSAGES/messages.mo: \
	i18n/ja/LC_MESSAGES/messages.po

$(topbuild_dir)i18n/ja.UTF-8/LC_MESSAGES/messages.mo: \
	i18n/ja.UTF-8/LC_MESSAGES/messages.po


$(topbuild_dir)%.mo : %.po
	$(MKDIR_P) $(@D);
	$(MSGFMT) -o $@	 $<

$(topbuild_dir)%.o : %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@  $<

clean-i18n-msg-objects:
	$(RM_R) $(topbuild_dir)i18n

clean-i18n-objects:
	$(RM) $(i18n_msg_OBJECTS)

clean-i18n:
	$(RM) $(topbuild_dir)i18n-msg

clean: clean-i18n-objects clean-i18n clean-i18n-msg-objects


.PHONY: clean clean-i18n clean-i18n-objects 

# vi: se ts=4 sw=4 noet:
