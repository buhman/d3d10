IMAGES += image/0_shipple2.DDS
image/0_shipple2.DDS: scenes/ship20/./images/0_shipple2.png
	@mkdir -p image
	texconv10.exe -f BC1_UNORM -nologo "$<"
	mv "$(<:.png=.DDS)" "$@"

