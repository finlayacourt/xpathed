{
    "targets": [
        {
            "target_name": "xpathed",
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "sources": ["src/binding.cpp", "src/pugixml.cpp"],
            "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
            "defines": [],
            "conditions": [
                [
                    'OS=="win"',
                    {
                        "defines": ["_HAS_EXCEPTIONS=1"],
                        "msvs_settings": {"VCCLCompilerTool": {"ExceptionHandling": 1}},
                    },
                ],
                [
                    'OS=="mac"',
                    {
                        "xcode_settings": {
                            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                            "CLANG_CXX_LIBRARY": "libc++",
                            "MACOSX_DEPLOYMENT_TARGET": "10.7",
                        }
                    },
                ],
            ],
        }
    ]
}
