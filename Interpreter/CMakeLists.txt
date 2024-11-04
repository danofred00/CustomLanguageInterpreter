# CMakeList.txt : projet CMake pour Compiler, incluez la source et définissez
# la logique spécifique au projet ici.
#


# Ajoutez une source à l'exécutable de ce projet.
add_executable (
	Interpreter 
	"src/main.cpp" 
	"src/lexer.cpp"
	"src/utils.cpp"
	"src/ast.cpp"
	"src/parser.cpp"
	"src/runtime/interpreter.cpp"
)

target_include_directories(Interpreter PUBLIC "include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Interpreter PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
