
function(add_turbo_test name srcs)
    add_sample(${name} ${srcs})
endfunction()

add_turbo_test(TurboAPIDesign_Descriptor ./TurboAPIDesign_Descriptor.cpp)
add_turbo_test(FlagsTest ./FlagsTest.cpp)