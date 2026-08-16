PROJECTS := libcronus cronus

.PHONY: all
all:
	@$(foreach project, $(PROJECTS), make -C $(project);)

.PHONY: clean
clean:
	@$(foreach project, $(PROJECTS), make -C $(project) clean;)

.PHONY: bear
bear: clean
	@$(foreach project, $(PROJECTS), make -C $(project) bear;)
