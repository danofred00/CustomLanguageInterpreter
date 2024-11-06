# Define a custom function that's can help us to add a function to the Interpreter target
function(add_function file)
    target_sources(Interpreter PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${file}.cpp")
endfunction()

# Add all Native functions files
add_function("io")
add_function("math")