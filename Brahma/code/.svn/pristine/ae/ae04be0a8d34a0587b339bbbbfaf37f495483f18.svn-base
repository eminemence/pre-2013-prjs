 #
 # Copyright (C) 2011-2012  Mayur Kankanwadi(eminemence@gmail.com)
 # This program is free software: you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 #  any later version.
 # 
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 # 
 # You should have received a copy of the GNU General Public License
 # along with this program.  If not, see <http://www.gnu.org/licenses/>
 # 


ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
CDIR=$(EPOCROOT)epoc32\winscw\c
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif

TARGETDIR=$(ZDIR)\resource\apps
ICONTARGETFILENAME=$(TARGETDIR)\ScrShotsAutomatic.mif

# For the icons used by the application code (engine in this case)
HEADERDIR=$(EPOCROOT)epoc32\include
TARGETDIR_PRIVATE=$(ZDIR)\private\2006508E
ICONTARGETFILENAME_PRIVATE=$(TARGETDIR_PRIVATE)\ScrShotsScreenIcons.mif
HEADERFILENAME_PRIVATE=$(HEADERDIR)\ScrShotsScreenIcons.mbg
ABOUTHTMLSOURCE_PRIVATE=$(CDIR)\\private\2006508E
FONTFILE_PRIVATE=$(CDIR)\scrshotfonts

ICONDIR=..\gfx

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN :
	@echo ...Deleting $(ICONTARGETFILENAME)
	del /q /f $(ICONTARGETFILENAME)
	@echo $(ICONTARGETFILENAME_PRIVATE)
	del /q /f $(ICONTARGETFILENAME_PRIVATE)

LIB : do_nothing

CLEANLIB : do_nothing

# ----------------------------------------------------------------------------
# NOTE: if you have JUSTINTIME enabled for your S60 3rd FP1 or newer SDK
# and this command crashes, consider adding "/X" to the command line.
# See <http://forum.nokia.com/document/Forum_Nokia_Technical_Library_v1_35/contents/FNTL/Build_process_fails_at_mif_file_creation_in_S60_3rd_Ed_FP1_SDK.htm>
# ----------------------------------------------------------------------------

RESOURCE : $(ICONTARGETFILENAME)

$(ICONTARGETFILENAME) : $(ICONDIR)\qgn_menu_ScrShotsAutomatic.svg
	mifconv $(ICONTARGETFILENAME) \
		/c32 $(ICONDIR)\brahma_icon.svg


	mifconv $(ICONTARGETFILENAME_PRIVATE) /h$(HEADERFILENAME_PRIVATE)  /c32,8 ..\gfx\number_0.svg /c32,8 ..\gfx\number_1.svg /c32,8 ..\gfx\number_2.svg /c32,8 ..\gfx\number_3.svg /c32,8 ..\gfx\number_4.svg /c32,8 ..\gfx\number_5.svg /c32,8 ..\gfx\number_6.svg /c32,8 ..\gfx\number_7.svg /c32,8 ..\gfx\number_8.svg /c32,8 ..\gfx\number_9.svg /c32,8 ..\gfx\refresh.svg /c32,8 ..\gfx\back.svg /c32,8 ..\gfx\sound_off.svg /c32,8 ..\gfx\sound_on.svg /c32,8 ..\gfx\app_scrshot_till_exit.svg /c32,8 ..\gfx\single_scrshot.svg /c32,8 ..\gfx\disabled_page_forward.svg /c32,8 ..\gfx\back_applist.svg /c32,8 ..\gfx\app_scrshot_for_duration.svg /c32,8 ..\gfx\info_icon.svg /c32,8 ..\gfx\f_drv_selected.svg /c32,8 ..\gfx\f_drv_unselected.svg /c32,8 ..\gfx\bmp_select.svg /c32,8 ..\gfx\stop_action.svg /c32,8 ..\gfx\disabled_page_back.svg /c32,8 ..\gfx\app_icon.svg /c32,8 ..\gfx\page_back.svg /c32,8 ..\gfx\multi_scrshot.svg /c32,8 ..\gfx\page_forward.svg /c32,8 ..\gfx\c_drv_selected.svg /c32,8 ..\gfx\e_drv_selected.svg /c32,8 ..\gfx\c_drv_unselected.svg /c32,8 ..\gfx\e_drv_unselected.svg /c32,8 ..\gfx\app_default_icon.svg /c32,8 ..\gfx\jpeg_unselected.svg /c32,8 ..\gfx\gif_unselected.svg /c32,8 ..\gfx\app_selected.svg /c32,8 ..\gfx\png_selected.svg /c32,8 ..\gfx\folder_goinside.svg /c32,8 ..\gfx\png_unselected.svg /c32,8 ..\gfx\folder_select_action.svg /c32,8 ..\gfx\folder_select.svg /c32,8 ..\gfx\folder_unselect.svg /c32,8 ..\gfx\bmp_selected.svg /c32,8 ..\gfx\bmp_unselected.svg /c32,8 ..\gfx\jpeg_selected.svg /c32,8 ..\gfx\gif_selected.svg /c32,8 ..\gfx\drv_select_action.svg /c32,8 ..\gfx\exit.svg /c32,8 ..\gfx\folder_up.svg /c32,8 ..\gfx\rename_folder.svg  /c32,8 ..\gfx\app_unselected.svg /c32,8 ..\gfx\d_drv_selected.svg /c32,8 ..\gfx\d_drv_unselected.svg
	copy ..\data\about.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\man_page.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\menu.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\presence.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\credit.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\faq.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\app_usage.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\idea.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\presence.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\bug.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\rate.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\manual.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\app_tips.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\drive_tips.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\folder_tips.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\help_tips.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\main_tips.html $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\brahma_icon.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\infostyles.css $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\form_load.js $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\star-gold16.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\drive_info.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\file_format.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\file_name_info.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\folder_info.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\folder_selection_option.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\number_edit_ctrl.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\options_area.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\stop_action.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\actions_explained.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\app_list_options_area.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\app_list_item_explained.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\drive_grid_item_explained.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\drive_grid_options_area.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\folder_list_item_explained.png $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\data\folder_selection_screen_options_area.png $(ABOUTHTMLSOURCE_PRIVATE)
	
	copy ..\sounds\tick.wav $(ABOUTHTMLSOURCE_PRIVATE)
	copy ..\font\octin_stencil_free.gdr $(FONTFILE_PRIVATE)
	
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing

