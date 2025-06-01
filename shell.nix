{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {    
    nativeBuildInputs = with pkgs.buildPackages; [
      # Build tools
      astyle
      cmake
      cmake-format
      bash
      curl
      git
      valgrind
      gdb
      protobuf_21
      SDL2
      SDL2_image

      # used in mbedtls build
      python312
      python312Packages.jsonschema
    ];
  }
