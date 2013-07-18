CXX = g++-4.8
CXX_VERSION = c++1y
OPTIMIZATION = -O3
INCLUDE_PATH = . ./inc
CXXFLAGS = -std=$(CXX_VERSION) $(addprefix -I, $(INCLUDE_PATH)) $(OPTIMIZATION)
LDLIBS =
TEST_LDLIBS = -lcspec

CP = cp -f

define find
	$(shell find $1 -name '*.cpp' -not -name '.*' $2 | sed -e 's/.cpp$$//')
endef

define binaries
	$(addsuffix .out, $1)
endef

define depends
	$(addsuffix .d, $1)
endef

define objects
	$(addsuffix .o, $1)
endef


MAIN_TARGETS = src/tssplitter
MAIN_FILES = $(call find, src)
MAIN_FILES_WITHOUT_MAIN = $(call find, src, $(addprefix -not -name , $(addsuffix .cpp, $(notdir $(MAIN_TARGETS)))))

TEST_TARGETS = $(call find, spec)
TEST_FILES = $(TEST_TARGETS)

ALL_TARGETS = $(MAIN_TARGETS) $(TEST_TARGETS)
ALL_FILES = $(MAIN_FILES) $(TEST_FILES)

ALL_BINARIES = $(call binaries, $(ALL_TARGETS))
ALL_OBJECTS  = $(call objects, $(ALL_FILES))
ALL_DEPENDS  = $(call depends, $(ALL_FILES))

REAL_TEST_BASE_PATH = tmp/out/tokyo_mx_test


.DEFAULT: all
.PHONY: all
all: override NO_EXCEC_TEST = 1
all: main test

.PHONY: main
main: depends $(call binaries, $(MAIN_TARGETS))

.PHONY: exec
exec: main
	$(foreach cmd, $(call binaries, $(MAIN_TARGETS)), ./$(cmd) $(args);)

.PHONY: test
test: override LDLIBS += $(TEST_LDLIBS)
test: $(call binaries, $(TEST_TARGETS))
	$(if $(NO_EXCEC_TEST),, $(foreach cmd, $(call binaries, $(TEST_TARGETS)), ./$(cmd);))

.PHONY: real_test
real_test: main stop_real_test
	$(RM) "$(REAL_TEST_BASE_PATH).*"
	recpt1 --b25 --strip --sid hd 16 - - 2>> $(REAL_TEST_BASE_PATH).log | ./tssplitter.out - $(REAL_TEST_BASE_PATH).ts 2>> $(REAL_TEST_BASE_PATH).log & \
	echo $$! > $(REAL_TEST_BASE_PATH).pid

.PHONY: stop_real_test
stop_real_test:
	if [ -f $(REAL_TEST_BASE_PATH).pid ]; then kill $$(cat $(REAL_TEST_BASE_PATH).pid); fi

.PHONY: depends
depends: $(ALL_DEPENDS)

.PHONY: count
count:
	find . -name "*.cpp" -not -name ".*" -or -name "*.hpp" -not -name ".*" | xargs wc -l

.PHONY: clean
clean:
	$(RM) $(ALL_BINARIES) $(ALL_OBJECTS)

.PHONY: distclean
distclean: clean
	$(RM) $(ALL_DEPENDS)

.PHONY: rebuild
rebuild: clean all

.PHONY: install
install: main
	$(foreach cmd, $(MAIN_TARGETS), $(CP) $(cmd).out ../../bin/$(notdir $(cmd)))


src/%.out: $(call objects, $(MAIN_FILES))
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

spec/%.out: spec/%.o $(call objects, $(MAIN_FILES_WITHOUT_MAIN))
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.d: %.cpp
	@printf $(dir $<) > $@
	$(CXX) -MM $(CXXFLAGS) $< >> $@


-include $(ALL_DEPENDS)
