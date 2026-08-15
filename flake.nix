{
	description = "Cronus development shell";

	inputs = {
		nixpkgs.url = "github:Nixos/nixpkgs/nixos-unstable";
	};

	outputs = { self, nixpkgs }:
	let
		system = "x86_64-linux";
		pkgs = nixpkgs.legacyPackages.${system};
	in
	{
		devShells.${system}.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
			packages = with pkgs; [
				git
				gnumake
				bear
			];

			shellHook = ''
				export FLAKE=1
			'';
		};
	};
}
