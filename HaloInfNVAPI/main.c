#pragma comment(lib, "nvapi/amd64/nvapi64.lib")
#include <Windows.h>
#include <nvapi.h>
#include <NvApiDriverSettings.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("HaloInfNVAPI.exe <Option> <Value>\n- <Option>: <Values>\n- FPS: 20 - 1000\n- LODBias: 0/-1/-1.5/-2/-2.5/-3\n");
		return 1;
	};
	char* opt = _strlwr(argv[1]);
	char* val = _strlwr(argv[2]);
	NvDRSSessionHandle hSession;
	NvDRSProfileHandle hProfile;
	NVDRS_SETTING sVSync = { .version = NVDRS_SETTING_VER };
	
	NVDRS_SETTING sFramerateLimiter = {
		.version = NVDRS_SETTING_VER,
		.settingId = FRL_FPS_ID,
		.settingType = NVDRS_DWORD_TYPE };

	NVDRS_SETTING sLODBiasDX = {
		.version = NVDRS_SETTING_VER,
		.settingId = LODBIASADJUST_ID,
		.settingType = NVDRS_DWORD_TYPE };

	NVDRS_SETTING sSupersampling = {
		.version = NVDRS_SETTING_VER,
		.settingId = AA_MODE_REPLAY_ID,
		.settingType = NVDRS_DWORD_TYPE };

	if (NvAPI_Initialize() ||
		NvAPI_DRS_CreateSession(&hSession) ||
		NvAPI_DRS_LoadSettings(hSession) ||
		NvAPI_DRS_FindProfileByName(hSession, L"Halo Infinite", &hProfile)) {
		printf("Error: Failed to initialize NvAPI!\n");
		return 1;
	}
	
	if (NvAPI_DRS_GetSetting(hSession, hProfile, VSYNCMODE_ID, &sVSync)) {
		printf("Error: Failed to get V-Sync Setting.\n");
		return 1;
	};
	
	if (sVSync.u32CurrentValue != VSYNCMODE_FORCEOFF) {
		sVSync.u32CurrentValue = VSYNCMODE_FORCEOFF;
		if (NvAPI_DRS_SetSetting(hSession, hProfile, &sVSync)) {
			printf("Error: Failed to set V-Sync to Force Off.\n");
			return 1;
		}
	}

	if (!strcmp(opt, "lodbias")) {
		if (!strcmp(val, "0"))
			sLODBiasDX.u32CurrentValue = 0;
		else if (!strcmp(val, "-1"))
			sLODBiasDX.u32CurrentValue = 0xFFFFFFF8;
		else if (!strcmp(val, "-1.5"))
			sLODBiasDX.u32CurrentValue = 0xFFFFFFF4;
		else if (!strcmp(val, "-2"))
			sLODBiasDX.u32CurrentValue = 0xFFFFFFF0;
		else if (!strcmp(val, "-2.5"))
			sLODBiasDX.u32CurrentValue = 0xFFFFFFEC;
		else if (!strcmp(val, "-3"))
			sLODBiasDX.u32CurrentValue = 0xFFFFFFE8;
		else {
			printf("Error: Invalid LOD Bias Value!\n");
			return 1;
		};

		if (sLODBiasDX.u32CurrentValue)
			sSupersampling.u32CurrentValue = AA_MODE_REPLAY_MODE_ALL;

		if (NvAPI_DRS_SetSetting(hSession, hProfile, &sLODBiasDX) ||
			NvAPI_DRS_SetSetting(hSession, hProfile, &sSupersampling)) {
			printf("Error: Failed to set LOD Bias!\n");
			return 1;
		};
		printf("LOD Bias set to %s.\n", val);
	}
	else if (!strcmp(opt, "fps")) {
		if (strspn(val, "0123456789") == strlen(val)) {
			sFramerateLimiter.u32CurrentValue = atoi(val);

			if (sFramerateLimiter.u32CurrentValue >= 20 &&
				sFramerateLimiter.u32CurrentValue <= 1000 ||
				!sFramerateLimiter.u32CurrentValue) {
				if (NvAPI_DRS_SetSetting(hSession, hProfile, &sFramerateLimiter)) {
					printf("Error: Failed to set Framerate Limiter!\n");
					return 1;
				};
				printf("Framerate Limiter set to %s FPS.\n", val);
			}
			else {
				printf("Error: Invalid Framerate Limiter Value!\n");
				return 1;
			};
		}
		else {
			printf("Error: Invalid Framerate Limiter Value.");
			return 1;
		};
	}
	else {
		printf("Error: Invalid Option!\n");
	};

	NvAPI_DRS_SaveSettings(hSession);
	NvAPI_DRS_DestroySession(hSession);
	NvAPI_Unload();
	return 0;
}