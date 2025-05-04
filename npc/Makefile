all:
	@echo "Write this Makefile by your self."

.PHONY: sim

sim: $(addprefix sim_, $(filter-out sim, $(MAKECMDGOALS)))

sim_%:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "[SIM] Building: $*"
	verilator --cc --exe --build -j 0 --trace -Wall csrc/$*_main.cpp vsrc/$*.v
	@echo "[SIM] Running: $*"
	./obj_dir/V$*

# sim:
# 	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
# 	@echo "Write this Makefile by your self."

include ../Makefile
