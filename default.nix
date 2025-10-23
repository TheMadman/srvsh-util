let
	pkgs = import <nixpkgs> {};
	srvsh = import (pkgs.fetchFromGitHub {
		owner = "TheMadman";
		repo = "srvsh";
		rev = "617beb9145590a78504b2721b7deaeba3902d69d";
		hash = "sha256-yp//WSfS6r9db6osLh3uxVXhb2yv4tFALM6D+Dd+mkE=";
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
