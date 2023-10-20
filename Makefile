.PHONY: build
build:
	@echo "Building..."
	docker build -t tlpi .

.PHONY: run
run: build
	docker rm tlpi || true
	docker run -it --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --name tlpi tlpi

.PHONY: login
login:
	docker exec -it tlpi /bin/bash
