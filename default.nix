let
	pkgs = import <nixpkgs> {};
	srvsh = import (pkgs.fetchFromGitHub {
		owner = "TheMadman";
		repo = "srvsh";
		rev = "14ba233480015396f5d9fe6858e7c53f6a3c478f";
		hash = "sha256-dQEMdG/CsbQIVDA+qyZ3BXLCHPFCPAnpss+5qu1wGWc=";
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
