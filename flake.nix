{
  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.nixpkgs.url = "nixpkgs/nixos-23.05";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};

          libbw64_pkg = { stdenv, cmake, ninja }:
            stdenv.mkDerivation {
              name = "libbw64";
              src = ./.;
              nativeBuildInputs = [ cmake ninja ];
              doCheck = true;
            };
        in
        rec {
          packages = rec {
            libbw64 = pkgs.callPackage libbw64_pkg { };

            default = libbw64;
          };

          devShells = rec {
            libbw64 = packages.libbw64.overrideAttrs (attrs: {
              nativeBuildInputs = attrs.nativeBuildInputs ++ [
                pkgs.clang-tools
                pkgs.nixpkgs-fmt
              ];
            });

            default = libbw64;
          };
        });
}

