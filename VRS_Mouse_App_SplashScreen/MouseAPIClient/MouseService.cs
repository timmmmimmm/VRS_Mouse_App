using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Json;
using System.Text;
using System.Threading.Tasks;
using VRS_Mouse_App_SplashScreen.MouseAPIClient.Entities;

namespace VRS_Mouse_App_SplashScreen.MouseAPIClient
{
    public sealed class MouseService
    {
        private readonly HttpClient _httpClient;

        public MouseService(HttpClient httpClient)
        {
            _httpClient = httpClient;
        }

        public async Task<Mouse?> GetMouseInfoAsync()
        {
            var content = await _httpClient.GetFromJsonAsync<Mouse>("http://localhost:12345/ma/api/all", 
                new System.Text.Json.JsonSerializerOptions()
            {
                PropertyNameCaseInsensitive = true
            });
            return content;
        }
    }
}
