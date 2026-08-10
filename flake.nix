{
        description = "Cronus development shell";

        inputs = {
                nixpkgs.url = "github:Nixos/nixpkgs/nixos-unstable";
        };

        outputs = { self, nixpkgs }:
                let
                        system = "x86_64-linux";
                        pkgs = nixpkgs.legacyPackages.${system};
                        shellName = "cronus-shell";
                in
                {
                        devShells.${system}.default = pkgs.mkShell {
                                name = shellName;

                                packages = with pkgs; [
                                        git
                                        clang
                                        gnumake
                                ];

                                shellHook = ''
                                        export FLAKE_NAME="${shellName}"
                                '';
                        };
                };
}
