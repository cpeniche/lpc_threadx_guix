

define dependencies
  echo "building dependency file : $(1)";\
  $(CC) -MM -MT$(1) -MT$(2) $(CFLAGS) $(INCLUDES) $< > $(1)
endef

define compile
  echo "[CC]: $(2)";\
  $(1) -c $(CFLAGS) $(2) $(INCLUDES) -o $(3)
endef