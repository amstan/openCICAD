# openCICAD Documentation Build Process
# 
# Targets:
#
#     make spec    - Convert all markdown specification into html
#     make clean   - Removes all "binary" html files
#     make package - Builds a portable documentation package
#

# Specification documentation
IMAGES = images/
CSS    = css/
INDEX  = readme.md
STYLE  = main.css

# Gather all of the required markdown files, index is a special case.
SRC = $(wildcard $(SPEC)*.md)

# Each markdown file represents one html file
BIN = $(patsubst %.md, %.html, $(SRC))

# The output destination of the packaged documentation
PACK_DEST = openCICAD/
PACK_ZIP = openCICAD.zip

spec: $(BIN)
	@echo "Success"

%.html: %.md
	@echo "Preparing $@"
	@sed 's/.md/.html/' $^ | pandoc -s -c $(CSS)$(STYLE) > $@
	
clean:
	@rm -f $(BIN)
	@echo "HTML Files Cleaned"
	@rm -rf $(PACK_DEST)
	@echo "Removed the documentation package"
	@rm -rf $(PACK_ZIP)
	@echo "Removed the zipped documentation package"

package: spec
	@echo "Creating a documentation package."
	@echo "Creating $(PACK_DEST) output directory structure."
	@mkdir -p $(PACK_DEST)
	@mkdir -p $(PACK_DEST)$(IMAGES)
	@mkdir -p $(PACK_DEST)$(CSS)
	@echo "Moving compiled documentation into the package."
	@-cp *.html $(PACK_DEST)$(SPEC) 2> /dev/null
	@-cp $(SPEC)*.html $(PACK_DEST)$(SPEC) 2> /dev/null
	@echo "Moving images into the package."
	@-cp $(IMAGES)* $(PACK_DEST)$(IMAGES) 2> /dev/null
	@echo "Moving the css into the package."
	@-cp $(CSS)$(STYLE) $(PACK_DEST)$(CSS) 2> /dev/null
	@echo "Documentation Compilation Success!"
	
zippackage: package $(PACK_ZIP)

$(PACK_ZIP): 
	@echo "Creating a zip file of the packaged documentation."
	@zip -r $(PACK_ZIP) $(PACK_DEST)
	@echo "$(PACK_ZIP) created successfully."

