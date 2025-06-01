{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {    
    nativeBuildInputs = with pkgs.buildPackages; [
      # Build tools
      cmake
      cmake-format
      bash
      curl
      git
      valgrind
      gdb
      protobuf_21

      # used in mbedtls build
      python312
      python312Packages.jsonschema
    ];
  }
