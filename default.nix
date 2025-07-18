let
	pkgs = import <nixpkgs> {};
	srvsh = import (pkgs.fetchFromGitHub {
		owner = "TheMadman";
		repo = "srvsh";
		rev = "f685f463877ae3a438eff85c376497c8cd41a34f";
		hash = "sha256-7wL/IIfkcTUq7SXKP/Sw1dXXCV/QO3NYmQICP9OnYOU=";
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
