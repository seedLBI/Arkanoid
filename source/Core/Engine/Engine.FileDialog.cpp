#include "Engine.h"

namespace engine {

	namespace filedialog {

		std::optional<std::string> OpenSaveDialog(
			const std::string& nameWindow,
			const std::vector<engine::filedialog::FileDialogFilter>& input_filters) {


			nfdu8char_t* outPath = nullptr;

			std::vector<std::string> specs_saved(input_filters.size());

			std::vector<nfdu8filteritem_t> filters(input_filters.size());
			for (size_t i = 0; i < input_filters.size(); i++) {
				filters[i].name = input_filters[i].name.c_str();
				filters[i].spec = input_filters[i].extensions.c_str();
			}

			nfdsavedialogu8args_t args = { 0 };

			args.filterList = filters.data();
			args.filterCount = input_filters.size();

			args.defaultName = "untitled";

			bool handleResult = NFD_GetNativeWindowFromGLFWWindow(
				engine::window::GetHandle(),
				&args.parentWindow
			);

			if (handleResult != true) {
				puts("Failed to get native window handle");

				return std::nullopt;
			}
			else {
				nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);

				if (result == NFD_OKAY) {

					std::string output = outPath;
					NFD_FreePathU8(outPath);


					return output;
				}
				else {
					if (outPath != nullptr) {
						NFD_FreePathU8(outPath);
					}


					return std::nullopt;
				}
			}


			return std::nullopt;
		}
	


	std::optional<std::string> OpenLoadDialog(
		const std::string& nameWindow,
		const std::vector<engine::filedialog::FileDialogFilter>& input_filters) {

		nfdu8char_t* outPath = nullptr;

		std::vector<std::string> specs_saved(input_filters.size());

		std::vector<nfdu8filteritem_t> filters(input_filters.size());
		for (size_t i = 0; i < input_filters.size(); i++) {
			filters[i].name = input_filters[i].name.c_str();
			filters[i].spec = input_filters[i].extensions.c_str();
		}

		nfdopendialogu8args_t args = { 0 };

		args.filterList = filters.data();
		args.filterCount = input_filters.size();


		bool handleResult = NFD_GetNativeWindowFromGLFWWindow(
			engine::window::GetHandle(),
			&args.parentWindow
		);

		if (handleResult != true) {
			puts("Failed to get native window handle");

			return std::nullopt;
		}
		else {
			nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

			if (result == NFD_OKAY) {

				std::string output = outPath;
				NFD_FreePathU8(outPath);


				return output;
			}
			else {
				if (outPath != nullptr) {
					NFD_FreePathU8(outPath);
				}


				return std::nullopt;
			}
		}


		return std::nullopt;
	}

	}
}