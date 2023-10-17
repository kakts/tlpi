.PHONY: build
build:
	@echo "Building..."
	docker build -t tlpi .

.PHONY: run
run: build
	docker run -it --cap-add=SYS_PTRACE --security-opt seccomp=unconfined tlpi