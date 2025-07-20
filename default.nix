let
	pkgs = import <nixpkgs> {};
	srvsh = import (pkgs.fetchFromGitHub {
		owner = "TheMadman";
		repo = "srvsh";
		rev = "df6e1404efab4c3db91a34b3b8c0aabc30a21d85";
		hash = "sha256-tRBfTDXaln+74nJhTAjX/aVcOr9EDq+uy0F3PNOXMIA=";
	});
in
pkgs.stdenv.mkDerivation {
	pname = "srvsh-utils";
	version = "0.0.1";
	src = ./.;

	buildInputs = [
		srvsh
	];

	installFlags = [
		"prefix=$(out)"
	];
}
