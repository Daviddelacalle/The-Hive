include MakeColors

BUILD	:= build/

SOURCES := $(shell find src/ -type f -name "*.cpp")
OBJECTS := $(patsubst %.cpp,%.o, $(SOURCES))
OBJECTS := $(addprefix $(BUILD),$(OBJECTS))

LIBRARIES := -lassimp

COMPILER := g++ -O3

EXE := BinaryFiles

build: $(EXE)

$(EXE): $(OBJECTS)
	$(call printWarning, "Preprocessing...")
	$(call br)
	@$(COMPILER) -o $@ $^ $(LIBRARIES)
	@./BinaryFiles
	$(call printObj,  "Done!")
	$(call br)
	$(call br)

$(OBJECTS): $(BUILD)%.o : %.cpp
	@mkdir -p ${@D}
	@$(COMPILER) -g -o $@ -c $<

clean:
	$(call printWarning, "Deleting" )
	$(call printObj,"BinaryFiles")
	$(call printWarning,"..." )
	@rm -rf $(BUILD)
	@rm -rf ./BinaryFiles
	@rm -rf ../assets/BinaryFiles/BinaryModels/*.modelgg
	@rm -rf ../assets/BinaryFiles/BoundingBoxes/*.bb
	$(call br)


info:
	$(call br)
	$(call printWarning, "Files:")
	$(call br)
	$(call br)
	$(call objColor)
	@$(foreach var,$(SOURCES), echo " "$(var); )
	$(call noColor)

	$(call br)
