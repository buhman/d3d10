IMAGES += image/american_cherry.DDS
image/american_cherry.DDS: C:/Program\ Files/Common\ Files/Autodesk\ Shared/Materials/Textures/1/Mats/american_cherry.png
	texconv10.exe -f BC1_UNORM -nologo "$<"
	mv "$(<:.png=.DDS)" "$@"

IMAGES += image/102.DDS
image/102.DDS: C:/Program\ Files/Common\ Files/Autodesk\ Shared/Materials/Textures/3/Mats/102.png
	texconv10.exe -f BC1_UNORM -nologo "$<"
	mv "$(<:.png=.DDS)" "$@"

IMAGES += image/Finishes.Flooring.Tile.Square.Medium\ Blue.DDS
image/Finishes.Flooring.Tile.Square.Medium\ Blue.DDS: C:/Program\ Files/Common\ Files/Autodesk\ Shared/Materials/Textures/3/Mats/Finishes.Flooring.Tile.Square.Medium\ Blue.png
	texconv10.exe -f BC1_UNORM -nologo "$<"
	mv "$(<:.png=.DDS)" "$@"

IMAGES += image/SiteWork.Planting.Grass.Bermuda1.DDS
image/SiteWork.Planting.Grass.Bermuda1.DDS: C:/Program\ Files/Common\ Files/Autodesk\ Shared/Materials/Textures/3/Mats/SiteWork.Planting.Grass.Bermuda1.jpg
	texconv10.exe -f BC1_UNORM -nologo "$<"
	mv "$(<:.jpg=.DDS)" "$@"

