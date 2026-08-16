PROJECTS := libcronus cronus

.PHONY: all
all:
	@$(foreach project, $(PROJECTS), $(MAKE) -C $(project);)

.PHONY: clean
clean:
	@$(foreach project, $(PROJECTS), $(MAKE) -C $(project) clean;)

.PHONY: bear
bear:
	@$(MAKE) -C libcronus bear
	@$(MAKE) -C libcronus bear
