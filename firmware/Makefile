

project_name := template
uvproj_path := ./MDK-ARM
.PHONY: default format clean flash debug

# Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

default:
	@echo 【START】----- UV build -----
	@- UV4 -b $(uvproj_path)/$(project_name).uvprojx -j0 -o ./Objects/Build_Output.txt
	@powershell -c type $(uvproj_path)/Objects/Build_Output.txt
	@echo 【 END 】----- UV build -----

clean:
	@echo 【START】----- UV clean -----
	UV4 -c $(uvproj_path)/$(project_name).uvprojx -j0
	@echo 【 END 】----- UV clean -----

flash:
	@echo 【START】----- UV flash -----
	@- UV4 -f $(uvproj_path)/$(project_name).uvprojx -j0 -o ./Objects/Program_Output.txt
	@powershell -c type $(uvproj_path)/Objects/Program_Output.txt
	@echo 【 END 】----- UV flash -----

debug:
	@echo 【START】----- UV debug -----
	UV4 -d $(uvproj_path)/$(project_name).uvprojx
	@echo 【 END 】----- UV debug -----

format:
	@echo Astyle source code format
	astyle --project=".astylerc" -r **.c,**.h --exclude=Objects -v -Q
